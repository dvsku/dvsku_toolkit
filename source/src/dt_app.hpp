#pragma once

#include "systems/dt_system_mngr.hpp"
#include "gui/dt_gui_mngr.hpp"

#include <libgui.hpp>

namespace dvsku_toolkit {
    class dt_app : public libgui::window {
    public:
        dt_system_mngr systems;
        dt_gui_mngr    gui;

    public:
        dt_app(const libgui::window_settings& settings);

    private:
        bool prepare() override final;

        void on_gui_before_update()	override final;
        void on_gui_update()		override final;
    };
}