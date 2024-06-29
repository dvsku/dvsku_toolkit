#pragma once

#include "systems/dt_sys_core.hpp"
#include "systems/dt_sys_evp.hpp"
#include "systems/dt_sys_command.hpp"

namespace dvsku_toolkit {
    class dt_app;

    class dt_system_mngr {
    public:
        dt_sys_core    core;
        dt_sys_evp     evp;
        dt_sys_command command;

    public:
        dt_system_mngr(dt_app& app);
    };
}
