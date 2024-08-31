#pragma once

#include "models/visual_tree/visual_tree_element.hpp"

#include <string>
#include <list>
#include <memory>
#include <functional>

namespace dvsku_toolkit {
    class visual_tree;
    class visual_tree_leaf;

    class visual_tree_branch : public visual_tree_element, public std::enable_shared_from_this<visual_tree_branch> {
    public:
        using branch_ptr_t = std::shared_ptr<visual_tree_branch>;
        using leaf_ptr_t   = std::shared_ptr<visual_tree_leaf>;
        using parent_ptr_t = std::weak_ptr<visual_tree_branch>;

        enum class check_state : uint8_t {
            unchecked,
            intermediate,
            checked
        };

    public:
        parent_ptr_t parent = {};

        std::list<branch_ptr_t> branches;
        std::list<leaf_ptr_t>   leaves;

    public:
        visual_tree_branch()                          = delete;
        visual_tree_branch(const visual_tree_branch&) = delete;
        visual_tree_branch(visual_tree_branch&&)      = delete;

        visual_tree_branch(const std::string& name);

        visual_tree_branch& operator=(const visual_tree_branch&) = delete;
        visual_tree_branch& operator=(visual_tree_branch&&)      = delete;

    public:
        check_state get_check_state() const;
        void check();

    private:
        using traverse_up_cb_t = std::function<void(branch_ptr_t)>;
        
        friend visual_tree;
        friend visual_tree_leaf;

    private:
        size_t m_leaf_count         = 0U;
        size_t m_leaf_visible_count = 0U;
        size_t m_leaf_checked_count = 0U;

    private:
        void traverse_up(traverse_up_cb_t callback);
    };
}
