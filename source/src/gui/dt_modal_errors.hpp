#pragma once

#include "gui/dt_gui_base.hpp"

#include <libgui.hpp>

namespace dvsku_toolkit {
    class dt_modal_errors : public dt_gui_base {
    public:
        dt_modal_errors(dt_app& app)
            : dt_gui_base(app) {}

    public:
        libgui::command::state render();
    };
}
