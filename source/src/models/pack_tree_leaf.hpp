#pragma once

#include "models/visual_tree/visual_tree_leaf.hpp"

namespace dvsku_toolkit {
    class pack_tree_leaf : public visual_tree_leaf {
    public:
        std::string file = "";

    public:
        pack_tree_leaf(const std::string& name)
            : visual_tree_leaf(name) {}
    };
}
