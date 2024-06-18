#include "dt_app.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace dvsku_toolkit;
using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_app::dt_app(const libgui::window_settings& settings)
    : window(settings), m_systems(*this), m_gui(*this)
{
    set_resizable(false);
}

dt_system_mngr& dt_app::get_systems() {
    return m_systems;
}

dt_gui_mngr& dt_app::get_gui() {
    return m_gui;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool dt_app::prepare() {
    set_borderless();

    theme::add_font("default", 13.0f, {
        font_average_mono,
        font_fontawesome_solid
    });

    m_systems.command.prepare();

    return true;
}

void dt_app::release() {

}

void dt_app::on_gui_before_update() {
    clear_frame_buffer();
}

void dt_app::on_gui_update() {
    m_gui.root.render();
}

void dt_app::on_gui_after_update() {

}