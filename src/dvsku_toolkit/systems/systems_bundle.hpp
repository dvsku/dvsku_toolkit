#pragma once

#include "systems/sys_evp.hpp"
#include "systems/sys_crypt.hpp"
#include "systems/sys_taskbar.hpp"

struct GLFWwindow;

namespace dvsku_toolkit {
    class systems_bundle {
    public:
        GLFWwindow** window = nullptr;

    public:
        sys_evp     evp;
        sys_crypt   crypt;
        sys_taskbar taskbar;

    public:
        systems_bundle();
    };
}
