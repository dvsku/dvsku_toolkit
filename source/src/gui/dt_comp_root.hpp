#pragma once

#include "gui/dt_gui_base.hpp"

namespace dvsku_toolkit {
    class dt_comp_root : public dt_gui_base {
    public:
        dt_comp_root()                    = delete;
        dt_comp_root(const dt_comp_root&) = delete;
        dt_comp_root(dt_comp_root&&)      = delete;

        dt_comp_root(dt_app& app);

        dt_comp_root& operator=(const dt_comp_root&) = delete;
        dt_comp_root& operator=(dt_comp_root&&)      = delete;

    public:
        void render();
    };
}