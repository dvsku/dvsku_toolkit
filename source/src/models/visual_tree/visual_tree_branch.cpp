#include "models/visual_tree/visual_tree_branch.hpp"
#include "models/visual_tree/visual_tree_leaf.hpp"

#include <stack>
#include <algorithm>

using namespace dvsku_toolkit;

visual_tree_branch::visual_tree_branch(const std::string& name)
    : visual_tree_element(name, element_type::branch) {}

visual_tree_branch::check_state visual_tree_branch::get_check_state() const {
    if (m_leaf_checked_count == m_leaf_count)
        return check_state::checked;
    else if (m_leaf_checked_count == 0)
        return check_state::unchecked;
    else
        return check_state::intermediate;
}

void visual_tree_branch::check() {
    check_state check_state = get_check_state();
    bool        set_checked = check_state != check_state::checked;

    std::stack<branch_ptr_t> stack;
    stack.push(this->shared_from_this());

    while (stack.size() != 0) {
        branch_ptr_t branch = stack.top();
        stack.pop();

        for (auto it = branch->branches.rbegin(); it != branch->branches.rend(); it++) {
            stack.push({ *it });
        }

        branch->m_leaf_checked_count = set_checked ? branch->m_leaf_count : 0U;

        for (leaf_ptr_t& leaf : branch->leaves) {
            leaf->m_checked = set_checked;
        }
    }

    traverse_up([&](branch_ptr_t up) {
        up->m_leaf_checked_count = 0U;

        for (const branch_ptr_t& child : up->branches)
            up->m_leaf_checked_count += child->m_leaf_checked_count;

        for (const leaf_ptr_t& child : up->leaves)
            if (child->is_checked())
                up->m_leaf_checked_count++;
    });
}

void visual_tree_branch::traverse_up(traverse_up_cb_t callback) {
    branch_ptr_t branch = this->parent.lock();

    while (branch) {
        if (callback)
            callback(branch);

        branch = branch->parent.lock();
    }
}
