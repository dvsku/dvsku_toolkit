#pragma once

#include "models/visual_tree/visual_tree_element.hpp"

#include <string>
#include <memory>
#include <functional>

namespace dvsku_toolkit {
    class visual_tree;
    class visual_tree_branch;

    class visual_tree_leaf : public visual_tree_element {
    public:
        using parent_ptr_t = std::weak_ptr<visual_tree_branch>;

    public:
        parent_ptr_t parent = {};

    public:
        visual_tree_leaf()                        = delete;
        visual_tree_leaf(const visual_tree_leaf&) = delete;
        visual_tree_leaf(visual_tree_leaf&&)      = delete;

        visual_tree_leaf(const std::string& name);

        visual_tree_leaf& operator=(const visual_tree_leaf&) = delete;
        visual_tree_leaf& operator=(visual_tree_leaf&&)      = delete;

    public:
        bool is_checked() const;
        void check();

    private:
        using branch_ptr_t     = std::shared_ptr<visual_tree_branch>;
        using traverse_up_cb_t = std::function<void(branch_ptr_t)>;
        
        friend visual_tree;
        friend visual_tree_branch;

    private:
        bool m_visible = true;
        bool m_checked = true;

    private:
        void traverse_up(traverse_up_cb_t callback);
    };
}
