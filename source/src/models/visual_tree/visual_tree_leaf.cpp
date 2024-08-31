#include "models/visual_tree/visual_tree_leaf.hpp"
#include "models/visual_tree/visual_tree_branch.hpp"

using namespace dvsku_toolkit;

visual_tree_leaf::visual_tree_leaf(const std::string& name)
    : visual_tree_element(name, element_type::leaf) {}

bool visual_tree_leaf::is_checked() const {
    return m_checked;
}

void visual_tree_leaf::check() {
    m_checked = !m_checked;

    traverse_up([&](branch_ptr_t branch) {
        if (m_checked)
            branch->m_leaf_checked_count++;
        else
            branch->m_leaf_checked_count--;
    });
}

void visual_tree_leaf::traverse_up(traverse_up_cb_t callback) {
    branch_ptr_t branch = this->parent.lock();

    while (branch) {
        if (callback)
            callback(branch);

        branch = branch->parent.lock();
    }
}
