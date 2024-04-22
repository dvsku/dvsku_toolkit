#pragma once

#include "systems/dt_sys_core.hpp"
#include "systems/dt_sys_evp.hpp"

namespace dvsku_toolkit {
    class dt_app;

    class dt_system_mngr {
    public:
        dt_sys_core core;
        dt_sys_evp  evp;

    public:
        dt_system_mngr()                      = delete;
        dt_system_mngr(const dt_system_mngr&) = delete;
        dt_system_mngr(dt_system_mngr&&)      = delete;

        dt_system_mngr(dt_app& app);

        dt_system_mngr& operator=(const dt_system_mngr&) = delete;
        dt_system_mngr& operator=(dt_system_mngr&&)      = delete;
    };
}