#include "gui/dt_comp_pack.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

void dt_comp_pack::render() {
    ImGui::PushID("Pack");
    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Input", &m_input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Input", { 125.0f, 21.0f })) {
        m_input = libgui::dialog::select_dir("Input dir", m_input);
    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Output", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Output", { 125.0f, 21.0f })) {
        m_output = libgui::dialog::save_file("Output evp", m_output, true, { "EVP (*.evp)", "*.evp" });
    }

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Pack type");
    ImGui::Unindent(3.0f);

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::RadioButton("any##Filter", &m_filter, 0);
    if (ImGui::IsItemHovered()) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
        ImGui::SetTooltip("Pack all files from input");
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);
    }

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("client##Filter", &m_filter, 1);
    if (ImGui::IsItemHovered()) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
        ImGui::SetTooltip("Pack only client files");
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);
    }   

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("server##Filter", &m_filter, 2);
    if (ImGui::IsItemHovered()) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
        ImGui::SetTooltip("Pack only server files");
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);
    }   

    ImGui::Dummy({ 0.0f, 10.0f });

    bool is_working = m_app.systems.core.work_context.is_working();

    if (is_working)
        ImGui::EndDisabled();

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_app.systems.core.work_context.get_progress() / 100, ImVec2(0.0f, 0.0f));

    ImGui::Dummy({ 0.0f, 5.0f });

    auto avail = ImGui::GetContentRegionMax();
    ImGui::SetCursorPosX((avail.x) / 2 - (125 / 2.0f));

    bool cannot_start = m_input.empty() || m_output.empty();

    if (cannot_start)
        ImGui::BeginDisabled();

    if (!is_working) {
        if (ImGui::Button("Pack##Pack", { 125.0f, 21.0f })) {
            m_app.systems.evp.pack(m_input, m_output, m_filter);
        }
    }
    else {
        if (ImGui::Button("Cancel##Cancel", { 125.0f, 21.0f })) {
            m_app.systems.core.work_context.cancel = true;
        }
    }

    if (cannot_start)
        ImGui::EndDisabled();

    if (is_working)
        ImGui::BeginDisabled();

    ImGui::Unindent(20.0f);
    ImGui::PopID();
}
