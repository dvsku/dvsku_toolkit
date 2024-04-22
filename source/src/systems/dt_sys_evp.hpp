#pragma once

#include "systems/dt_system_base.hpp"

namespace dvsku_toolkit {
    class dt_app;

    class dt_sys_evp : public dt_system_base {
    public:
        dt_sys_evp()                  = delete;
        dt_sys_evp(const dt_sys_evp&) = delete;
        dt_sys_evp(dt_sys_evp&&)      = delete;

        dt_sys_evp& operator=(const dt_sys_evp&) = delete;
        dt_sys_evp& operator=(dt_sys_evp&&)      = delete;

        dt_sys_evp(dt_app& app)
            : dt_system_base(app) {};
    };
}