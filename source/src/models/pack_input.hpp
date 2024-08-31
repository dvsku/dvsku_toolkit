#pragma once

#include "models/visual_tree/visual_tree.hpp"

#include <string>

namespace dvsku_toolkit {
    struct pack_input {
        std::string input        = "";
        std::string output       = "";
        std::string search_query = "";

        visual_tree tree = visual_tree(std::make_shared<visual_tree_branch>(""));

        void reset() {
            input        = "";
            output       = "";
            search_query = "";
            tree         = visual_tree(std::make_shared<visual_tree_branch>(""));
        }
    };
}
