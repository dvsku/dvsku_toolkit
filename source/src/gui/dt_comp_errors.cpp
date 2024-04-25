#include "gui/dt_comp_errors.hpp"
#include "dt_app.hpp"

#include <dv_gui_opengl\fonts\dv_font_fontawesome_solid.hpp>

using namespace dvsku_toolkit;
using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_comp_errors::dt_comp_errors(dt_app& app)
    : dt_gui_base(app) {}

dv_command_state dt_comp_errors::render() {
    dv_command_state state = dv_command_state::repeat;

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoNav;
    flags |= ImGuiWindowFlags_NoSavedSettings;
    flags |= ImGuiWindowFlags_NoDocking;
    flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowSize({350.0f, 400.0f}, ImGuiCond_Appearing);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 4.0f });

    bool* visible = &m_app.get_systems().command.is_set_to_execute(dt_commands::flag_show_error_window);
    if (ImGui::Begin("Errors##Component", visible, flags)) {
        ImGui::PushStyleColor(ImGuiCol_Button,        0x00FFFFFF);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xFF3D3D3D);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  0xFF3D3D3D);
        ImGui::PushStyleColor(ImGuiCol_Text,          0xFFC5C5C5);

        if (ImGui::Button(ICON_FA_COPY"##CopyToClipboard", { 30.0f, 25.0f })) {
            ImGui::SetClipboardText(m_app.get_systems().core.errors.c_str());
        }

        ImGui::PopStyleColor(4);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
        ImGui::SetItemTooltip("Copy to clipboard");
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);

        auto avail = ImGui::GetContentRegionAvail();
        auto max   = ImGui::GetContentRegionMax();

        if (ImGui::BeginChild("##Message", { avail.x, avail.y - 25.0f })) {
            auto avail = ImGui::GetContentRegionAvail();

            ImGui::TextWrapped(m_app.get_systems().core.errors.c_str());
        }
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::PopStyleVar(2);

    return state;
}
