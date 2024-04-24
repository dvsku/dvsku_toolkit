#pragma once

#include "systems\dt_system_mngr.hpp"
#include "gui\dt_gui_mngr.hpp"

#include <dv_gui_opengl/dv_window.hpp>
#include <dv_gui_opengl/models/dv_texture.hpp>

namespace dvsku_toolkit {
    class dt_app : public dvsku::dv_window {
    public:
        dt_app()              = delete;
        dt_app(const dt_app&) = delete;
        dt_app(dt_app&&)      = delete;

        dt_app(const dvsku::dv_window_settings& settings);

        dt_app& operator=(const dt_app&) = delete;
        dt_app& operator=(dt_app&&)      = delete;

    public:
        dt_system_mngr& get_systems();
        dt_gui_mngr& get_gui();

        void close();

    private:
        dt_system_mngr m_systems;
        dt_gui_mngr    m_gui;

        friend dt_comp_root;
        friend dt_comp_pack;
        friend dt_comp_unpack;

    private:
        bool prepare() override final;
        void release() override final;

        void on_gui_before_update()	override final;
        void on_gui_update()		override final;
        void on_gui_after_update()	override final;
    };
}