#pragma once

#include "systems/sys_evp.hpp"
#include "systems/sys_crypt.hpp"
#include "systems/sys_taskbar.hpp"
#include "systems/sys_sound.hpp"

struct GLFWwindow;

namespace dvsku_toolkit {
    class systems_bundle {
    public:
        GLFWwindow** window = nullptr;

    public:
        sys_evp     evp;
        sys_crypt   crypt;
        sys_taskbar taskbar;
        sys_sound   sound;

    public:
        systems_bundle();
    };
}
