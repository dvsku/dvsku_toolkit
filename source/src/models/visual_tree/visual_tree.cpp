#include "models/visual_tree/visual_tree.hpp"
#include "models/visual_tree/visual_tree_leaf.hpp"

#include <filesystem>
#include <stack>
#include <iostream>

using namespace dvsku_toolkit;

visual_tree::visual_tree(branch_ptr_t root)
    : root(root) {}

visual_tree::branch_ptr_t visual_tree::get_branch(const std::string& name, branch_ptr_t parent) {
    if (name == "")
        return nullptr;
    
    if (!parent) 
        parent = root;

    for (branch_ptr_t& branch : parent->branches) {
        if (branch->name == name)
            return branch;
    }

    return nullptr;
}

bool visual_tree::add_branch(branch_ptr_t branch, branch_ptr_t parent) {
    if (!branch || branch->name == "")
        return false;
    
    if (!parent)
        parent = root;

    if (get_branch(branch->name, parent))
        return false;

    branch->parent = parent->weak_from_this();
    parent->branches.push_back(branch);
    return true;
}

bool visual_tree::remove_branch(branch_ptr_t branch) {
    if (!branch)
        return false;

    branch_ptr_t parent = branch->parent.lock();
    if (!parent)
        return false;

    if (!parent->branches.remove(branch))
        return false;

    branch->traverse_up([&](branch_ptr_t up) {
        up->m_leaf_count         -= branch->m_leaf_count;
        up->m_leaf_visible_count -= branch->m_leaf_visible_count;
        up->m_leaf_checked_count -= branch->m_leaf_checked_count;
    });

    return true;
}

bool visual_tree::add_leaf(leaf_ptr_t leaf, branch_ptr_t parent) {
    if (!leaf)
        return false;
    
    if (!parent)
        parent = root;

    leaf->parent = parent->weak_from_this();
    parent->leaves.push_back(leaf);

    leaf->traverse_up([&](branch_ptr_t branch) {
        branch->m_leaf_count++;

        if (leaf->m_visible)
            branch->m_leaf_visible_count++;

        if (leaf->m_checked)
            branch->m_leaf_checked_count++;
    });

    return true;
}

bool visual_tree::remove_leaf(leaf_ptr_t leaf) {
    if (!leaf)
        return false;

    branch_ptr_t parent = leaf->parent.lock();
    if (!parent)
        return false;

    if (!parent->leaves.remove(leaf))
        return false;

    leaf->traverse_up([&](branch_ptr_t branch) {
        branch->m_leaf_count--;

        if (leaf->m_visible)
            branch->m_leaf_visible_count--;

        if (leaf->m_checked)
            branch->m_leaf_checked_count--;
    });

    return true;
}

bool visual_tree::is_hidden_by_search(branch_ptr_t branch) const {
    return branch && m_searching && branch->m_leaf_visible_count == 0;
}

bool visual_tree::is_hidden_by_search(leaf_ptr_t leaf) const {
    return leaf && m_searching && !leaf->m_visible;
}

void visual_tree::search(const std::string& query) {
    std::stack<visual_tree::branch_ptr_t> stack;
    m_searching = query != "";

    stack.push(root);

    while (stack.size() != 0) {
        visual_tree::branch_ptr_t branch = stack.top();
        stack.pop();

        for (auto it = branch->branches.rbegin(); it != branch->branches.rend(); it++) {
            stack.push({ *it });
        }

        branch->m_leaf_visible_count = 0U;

        if (branch->leaves.size() == 0)
            continue;

        for (visual_tree::leaf_ptr_t& leaf : branch->leaves) {
            leaf->m_visible = !m_searching || (m_searching && leaf->name.find(query) != std::string::npos);

            if (leaf->m_visible)
                branch->m_leaf_visible_count++;
        }

        branch->traverse_up([&](branch_ptr_t up) {
            up->m_leaf_visible_count += branch->m_leaf_visible_count;
        });
    }
}

void visual_tree::traverse(traverse_cb_t callback) {
    if (!callback) return;

    std::stack<visual_tree::branch_ptr_t> stack;
    stack.push(root);

    while (stack.size() != 0) {
        visual_tree::branch_ptr_t branch = stack.top();
        stack.pop();

        if (!branch)
            continue;

        for (auto it = branch->branches.rbegin(); it != branch->branches.rend(); it++) {
            stack.push({ *it });
        }

        for (const visual_tree::leaf_ptr_t& leaf : branch->leaves) {
            if (!leaf) continue;

            callback(leaf);
        }

        callback(branch);
    }
}
