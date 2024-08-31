#pragma once

#include "gui/dt_gui_base.hpp"

#include <string>

namespace dvsku_toolkit {
    class dt_comp_pack : public dt_gui_base {
    public:
        dt_comp_pack(dt_app& app)
            : dt_gui_base(app) {}

    public:
        void render();

    private:
        void render_tree();
    };
}
