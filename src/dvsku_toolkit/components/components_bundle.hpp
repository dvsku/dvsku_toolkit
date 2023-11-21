#pragma once

#include "systems/systems_bundle.hpp"
#include "components/comp_view.hpp"

namespace dvsku_toolkit {
    class components_bundle {
    public:
        systems_bundle& systems;

        comp_view view;

    public:
        components_bundle(systems_bundle& systems);
    };
}
