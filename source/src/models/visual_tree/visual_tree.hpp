#pragma once

#include "models/visual_tree/visual_tree_branch.hpp"
#include "models/visual_tree/visual_tree_leaf.hpp"

#include <string>
#include <functional>

namespace dvsku_toolkit {
    class visual_tree {
    public:
        using element_t     = visual_tree_element;
        using branch_t      = visual_tree_branch;
        using leaf_t        = visual_tree_leaf;
        using element_ptr_t = std::shared_ptr<visual_tree_element>;
        using branch_ptr_t  = std::shared_ptr<branch_t>;  
        using leaf_ptr_t    = std::shared_ptr<leaf_t>;
        
        using traverse_cb_t = std::function<void(element_ptr_t)>;

    public:
        branch_ptr_t root;

    public:
        visual_tree(branch_ptr_t root);

        visual_tree()                   = delete;
        visual_tree(const visual_tree&) = default;
        visual_tree(visual_tree&&)      = default;

        visual_tree& operator=(const visual_tree&) = default;
        visual_tree& operator=(visual_tree&&)      = default;

    public:
        branch_ptr_t get_branch(const std::string& name, branch_ptr_t parent = nullptr);
        bool add_branch(branch_ptr_t branch, branch_ptr_t parent = nullptr);
        bool remove_branch(branch_ptr_t branch);

        bool add_leaf(leaf_ptr_t leaf, branch_ptr_t parent = nullptr);
        bool remove_leaf(leaf_ptr_t leaf);

        bool is_hidden_by_search(branch_ptr_t branch) const;
        bool is_hidden_by_search(leaf_ptr_t leaf)     const;

        void search(const std::string& query);
        void traverse(traverse_cb_t callback);

    private:
        bool m_searching = false;
    };
}
