#pragma once

#include "systems/dt_system_base.hpp"

#include <dv_gui_opengl/systems/dv_sys_command.hpp>

namespace dvsku_toolkit {
    class dt_app;
    
    enum class dt_commands {
        flag_show_error_window
    };

    class dt_sys_command : public dt_system_base, public dvsku::dv_sys_command<dt_commands> {
    public:
        dt_sys_command()                      = delete;
        dt_sys_command(const dt_sys_command&) = delete;
        dt_sys_command(dt_sys_command&&)      = delete;

        dt_sys_command& operator=(const dt_sys_command&) = delete;
        dt_sys_command& operator=(dt_sys_command&&)      = delete;

        dt_sys_command(dt_app& app)
            : dt_system_base(app), dv_sys_command<dt_commands>() {};

    public:
        void prepare();
        void release();
    };
}