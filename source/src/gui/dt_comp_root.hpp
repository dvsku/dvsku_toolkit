#pragma once

#include "gui/dt_gui_base.hpp"

namespace dvsku_toolkit {
    class dt_comp_root : public dt_gui_base {
    public:
        dt_comp_root(dt_app& app)
            : dt_gui_base(app) {}

    public:
        void render();
    };
}
