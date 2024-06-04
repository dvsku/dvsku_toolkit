#pragma once

#include "gui/dt_gui_base.hpp"

#include <libgui.hpp>

namespace dvsku_toolkit {
    class dt_modal_errors : public dt_gui_base {
    public:
        dt_modal_errors()                       = delete;
        dt_modal_errors(const dt_modal_errors&) = delete;
        dt_modal_errors(dt_modal_errors&&)      = delete;

        dt_modal_errors(dt_app& app);

        dt_modal_errors& operator=(const dt_modal_errors&) = delete;
        dt_modal_errors& operator=(dt_modal_errors&&)      = delete;

    public:
        dvsku::gui_command_state render();
    };
}