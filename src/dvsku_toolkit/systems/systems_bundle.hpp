#pragma once

#include "systems/sys_evp.hpp"
#include "systems/sys_crypt.hpp"

namespace dvsku_toolkit {
    class systems_bundle {
    public:
        sys_evp   evp;
        sys_crypt crypt;

    public:
        systems_bundle();
    };
}
