#pragma once

#include <string>
#include <cstdint>

namespace dvsku_toolkit {
    class visual_tree_element {
    public:
        enum class element_type : uint8_t {
            branch,
            leaf
        };

    public:
        std::string        name = "";
        const element_type type;

    public:
        visual_tree_element(const std::string& name, element_type type)
            : name(name), type(type) {}
    };
}
