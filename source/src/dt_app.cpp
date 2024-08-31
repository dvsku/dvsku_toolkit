#include "dt_app.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace dvsku_toolkit;
using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_app::dt_app(const libgui::window_settings& settings)
    : window(settings), systems(*this), gui(*this)
{
    set_resizable(true);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool dt_app::prepare() {
    set_borderless();

    theme::add_font("default", 13.0f, {
        font_average_mono,
        font_fontawesome_solid
    });

    systems.command.prepare();

    return true;
}

void dt_app::on_gui_before_update() {
    clear_frame_buffer();
}

void dt_app::on_gui_update() {
    std::lock_guard guard(systems.core.mutex);

    gui.root.render();
}
