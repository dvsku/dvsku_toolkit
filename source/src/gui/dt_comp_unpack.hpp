#pragma once

#include "gui/dt_gui_base.hpp"

#include <string>
#include <mutex>
#include <libevp.hpp>

namespace dvsku_toolkit {
    class dt_comp_unpack : public dt_gui_base {
    public:
        dt_comp_unpack()                      = delete;
        dt_comp_unpack(const dt_comp_unpack&) = delete;
        dt_comp_unpack(dt_comp_unpack&&)      = delete;

        dt_comp_unpack(dt_app& app);

        dt_comp_unpack& operator=(const dt_comp_unpack&) = delete;
        dt_comp_unpack& operator=(dt_comp_unpack&&)      = delete;

    public:
        void render();

    private:
        std::string m_input    = "";
        std::string m_output   = "";
        int         m_filter   = 0U;
        float       m_progress = 0.0f;
        bool        m_cancel   = false;

        std::mutex          m_mutex;
        libevp::evp_context m_context;
    };
}