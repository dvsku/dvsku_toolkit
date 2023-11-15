#pragma once

#include "systems/systems_bundle.hpp"
#include "components/comp_root.hpp"

namespace dvsku_toolkit {
    class components_bundle {
    public:
        systems_bundle& systems;

        comp_root view;

    public:
        components_bundle(systems_bundle& systems);
    };
}
