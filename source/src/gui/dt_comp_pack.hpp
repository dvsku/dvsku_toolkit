#pragma once

#include "gui/dt_gui_base.hpp"

#include <string>

namespace dvsku_toolkit {
    class dt_comp_pack : public dt_gui_base {
    public:
        dt_comp_pack()                    = delete;
        dt_comp_pack(const dt_comp_pack&) = delete;
        dt_comp_pack(dt_comp_pack&&)      = delete;

        dt_comp_pack(dt_app& app);

        dt_comp_pack& operator=(const dt_comp_pack&) = delete;
        dt_comp_pack& operator=(dt_comp_pack&&)      = delete;

    public:
        void render();

    private:
        std::string m_input    = "";
        std::string m_output   = "";
        int         m_filter   = 0U;
        float       m_progress = 0.0f;
        bool        m_cancel   = false;
    };
}