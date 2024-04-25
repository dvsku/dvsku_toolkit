#pragma once

#include "gui/dt_gui_base.hpp"

#include <dv_gui_opengl/models/dv_command.hpp>

namespace dvsku_toolkit {
    class dt_comp_errors : public dt_gui_base {
    public:
        dt_comp_errors() = delete;
        dt_comp_errors(const dt_comp_errors&) = delete;
        dt_comp_errors(dt_comp_errors&&) = delete;

        dt_comp_errors(dt_app& app);

        dt_comp_errors& operator=(const dt_comp_errors&) = delete;
        dt_comp_errors& operator=(dt_comp_errors&&) = delete;

    public:
        dvsku::dv_command_state render();
    };
}