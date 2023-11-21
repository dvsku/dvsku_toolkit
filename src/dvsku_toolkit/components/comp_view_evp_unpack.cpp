#include "components/comp_view_evp_unpack.hpp"
#include "components/components_bundle.hpp"
#include "systems/systems_bundle.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "utilities/utilities_file_dialog.hpp"

using namespace dvsku_toolkit;

comp_view_evp_unpack::comp_view_evp_unpack(components_bundle& components) 
    : comp_evp_base(components) {}

void comp_view_evp_unpack::render() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF2E2E2E);

    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##InputUnpack", &m_input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##InputUnpack", { 125, 20 })) {
        file_dialog::open_file(m_input);
    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##OutputUnpack", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##OutputUnpack", { 125, 20 })) {
        file_dialog::select_folder(m_output);
    }

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::Checkbox("##UnpackDecrypt", &m_decrypt);
    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("Decrypt");

    ImGui::Dummy({ 0.0f, 3.0f });

    if (!m_decrypt)
        ImGui::BeginDisabled();

    ImGui::Indent(3.0f);
    ImGui::Text("Key");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::InputText("##UnpackKey", &m_key);

    ImGui::Indent(3.0f);
    ImGui::Text("IV");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(120.0f);
    ImGui::InputScalar("##UnpackIv", ImGuiDataType_U8, &m_iv);

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("(0 - 255)");

    if (!m_decrypt)
        ImGui::EndDisabled();

    ImGui::Dummy({ 0.0f, 68.0f });

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImVec2 avail = ImGui::GetContentRegionMax();
    ImGui::SetCursorPosX((avail.x) / 2 - (125 / 2.0f));

    if (!can_start())
        ImGui::BeginDisabled();

    if (ImGui::Button("Unpack##Unpack", { 125, 20 })) {
        m_components.systems.evp.set_start_callback([&]() { handle_on_start(); });
        m_components.systems.evp.set_finish_callback([&](bool success) { handle_on_finish(success); });
        m_components.systems.evp.set_update_callback([&](float value) { handle_on_update(value); });
        m_components.systems.evp.set_error_callback([&](const std::string& msg) { handle_on_error(msg); });

        m_components.systems.evp.unpack(m_input, m_output, m_decrypt, m_key, m_iv);
    }

    if (!can_start())
        ImGui::EndDisabled();

    ImGui::Unindent(20.0f);

    ImGui::PopStyleColor();
}

bool comp_view_evp_unpack::can_start() {
    bool result = !m_input.empty() && !m_output.empty();

    if (m_decrypt)
        result = result && !m_key.empty();

    return result;
}
