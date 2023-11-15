#include "components/components_bundle.hpp"

using namespace dvsku_toolkit;

components_bundle::components_bundle(systems_bundle& systems) 
    : systems(systems), view(*this) {}
