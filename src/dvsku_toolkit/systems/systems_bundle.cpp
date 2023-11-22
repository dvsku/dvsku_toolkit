#include "systems/systems_bundle.hpp"

using namespace dvsku_toolkit;

systems_bundle::systems_bundle() 
    : evp(*this), crypt(*this) {}
