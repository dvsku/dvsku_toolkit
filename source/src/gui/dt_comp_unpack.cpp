#include "gui/dt_comp_unpack.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_comp_unpack::dt_comp_unpack(dt_app& app)
    : dt_gui_base(app) {}

void dt_comp_unpack::render() {
    ImGui::PushID("Unpack");
    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Input", &m_input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Input", { 125.0f, 21.0f })) {
        //file_dialog::open_file(m_input);
    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Output", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Output", { 125.0f, 21.0f })) {
        //file_dialog::select_folder(m_output);
    }

    ImGui::Dummy({ 0.0f, 10.0f });

    bool is_working = m_app.get_systems().core.is_working;

    if (is_working)
        ImGui::EndDisabled();

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_progress / 100, ImVec2(0.0f, 0.0f));

    ImGui::Dummy({ 0.0f, 5.0f });

    ImVec2 avail = ImGui::GetContentRegionMax();
    ImGui::SetCursorPosX((avail.x) / 2 - (125 / 2.0f));

    bool cannot_start = m_input.empty() || m_output.empty() || !m_cancel;

    if (cannot_start)
        ImGui::BeginDisabled();

    if (!is_working) {
        if (ImGui::Button("Unpack##Unpack", { 125, 20 })) {
            //m_components.systems.evp.set_start_callback([&]() { handle_on_start(); });
            //m_components.systems.evp.set_finish_callback([&](bool success) { handle_on_finish(success); });
            //m_components.systems.evp.set_update_callback([&](float value) { handle_on_update(value); });
            //m_components.systems.evp.set_error_callback([&](const std::string& msg) { handle_on_error(msg); });

            //m_components.systems.evp.unpack(m_input, m_output, m_decrypt, m_key, m_iv);
        }
    }
    else {
        if (ImGui::Button("Cancel##Cancel", { 125, 20 })) {
            m_cancel = true;
        }
    }

    if (cannot_start)
        ImGui::EndDisabled();

    if (is_working)
        ImGui::BeginDisabled();

    ImGui::Unindent(20.0f);
    ImGui::PopID();
}
