#pragma once

#include "gui/dt_gui_base.hpp"

#include <string>

namespace dvsku_toolkit {
    class dt_comp_unpack : public dt_gui_base {
    public:
        dt_comp_unpack(dt_app& app)
            : dt_gui_base(app) {}

    public:
        void render();

    private:
        std::string m_input  = "";
        std::string m_output = "";
        int         m_filter = 0U;
    };
}
