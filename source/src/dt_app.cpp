#include "dt_app.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace dvsku_toolkit;
using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_app::dt_app(const dvsku::dv_window_settings& settings)
    : dv_window(settings), m_systems(*this), m_gui(*this)
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

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    config.RasterizerMultiply = 1.2f;
    config.SizePixels = 13.0f;

    ImGuiIO& io = ImGui::GetIO();

    // Add default font
    io.Fonts->AddFontFromMemoryTTF(AVERAGE_MONO, (int)AVERAGE_MONO_LEN, 13.0f, &config);

    // Add font awesome
    config.MergeMode = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromMemoryTTF(FONTAWESOME_SOLID, (int)FONTAWESOME_SOLID_LEN, 13.0f, &config, icon_ranges);

    // Build fonts
    io.Fonts->Build();

    ImGuiStyle& style  = ImGui::GetStyle();
    ImVec4*     colors = style.Colors;

    style.TabRounding              = 0.0f;
    style.TabBarBorderSize         = 2.0f;
    style.DockingSeparatorSize     = 1.0f;
    style.WindowBorderSize         = 0.0f;
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    colors[ImGuiCol_WindowBg]           = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
    colors[ImGuiCol_ChildBg]            = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
    colors[ImGuiCol_PopupBg]            = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
    colors[ImGuiCol_TitleBg]            = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_FrameBg]            = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_FrameBgActive]      = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_Button]             = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_ButtonHovered]      = ImLerp(colors[ImGuiCol_Button], ImVec4(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    colors[ImGuiCol_ButtonActive]       = ImLerp(colors[ImGuiCol_Button], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);
    colors[ImGuiCol_PlotHistogram]      = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_CheckMark]          = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_Tab]                = ImVec4(0.18039f, 0.18039f, 0.18039f, 1.00f);
    colors[ImGuiCol_TabHovered]         = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_TabUnfocused]       = ImVec4(0.18039f, 0.18039f, 0.18039f, 1.00f);
    colors[ImGuiCol_TabActive]          = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);

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