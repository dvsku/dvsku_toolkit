#pragma once

#include "gui/dt_comp_root.hpp"
#include "gui/dt_comp_pack.hpp"
#include "gui/dt_comp_unpack.hpp"

namespace dvsku_toolkit {
    class dt_app;

    class dt_gui_mngr {
    public:
        dt_comp_root   root;
        dt_comp_pack   pack;
        dt_comp_unpack unpack;

    public:
        dt_gui_mngr()                   = delete;
        dt_gui_mngr(const dt_gui_mngr&) = delete;
        dt_gui_mngr(dt_gui_mngr&&)      = delete;

        dt_gui_mngr(dt_app& app);

        dt_gui_mngr& operator=(const dt_gui_mngr&) = delete;
        dt_gui_mngr& operator=(dt_gui_mngr&&)      = delete;
    };
}