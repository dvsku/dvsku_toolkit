#pragma once

#include "systems/dt_system_base.hpp"

#include <dv_gui_opengl/models/dv_version.hpp>

namespace dvsku_toolkit {
    class dt_app;

    class dt_sys_core : public dt_system_base {
    public:
        dvsku::dv_version app_version;
        bool              is_working = false;
        bool              has_errors = false;
        std::string       errors     = "";

    public:
        dt_sys_core()                   = delete;
        dt_sys_core(const dt_sys_core&) = delete;
        dt_sys_core(dt_sys_core&&)      = delete;

        dt_sys_core& operator=(const dt_sys_core&) = delete;
        dt_sys_core& operator=(dt_sys_core&&)      = delete;

        dt_sys_core(dt_app& app);
    };
}