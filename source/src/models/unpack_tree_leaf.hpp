#pragma once

#include "models/visual_tree/visual_tree_leaf.hpp"

#include <libevp/model/evp_fd.hpp>

namespace dvsku_toolkit {
    class unpack_tree_leaf : public visual_tree_leaf {
    public:
        libevp::evp_fd file;

    public:
        unpack_tree_leaf(const std::string& name)
            : visual_tree_leaf(name) {}
    };
}
