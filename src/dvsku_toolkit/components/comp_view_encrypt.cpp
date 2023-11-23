#include "components/comp_view_encrypt.hpp"
#include "components/components_bundle.hpp"
#include "systems/systems_bundle.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "utilities/utilities_file_dialog.hpp"

using namespace dvsku_toolkit;

comp_view_encrypt::comp_view_encrypt(components_bundle& components) 
    : comp_crypt_base(components) {}

void comp_view_encrypt::render() {
    ImGui::PushID("Encrypt");

    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Input", &m_input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Input", { 125, 20 })) {
        file_dialog::select_folder(m_input);
    }

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::Checkbox("Encrypt to folder##ToDir", &m_encrypt_to_dir);

    ImGui::Dummy({ 0.0f, 3.0f });

    if (!m_encrypt_to_dir)
        ImGui::BeginDisabled();

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Output", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Output", { 125, 20 })) {
        file_dialog::select_folder(m_output);
    }

    if (!m_encrypt_to_dir)
        ImGui::EndDisabled();

    ImGui::Indent(3.0f);
    ImGui::Text("Key");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::InputText("##Key", &m_key);

    ImGui::Indent(3.0f);
    ImGui::Text("IV");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(120.0f);
    ImGui::InputScalar("##Iv", ImGuiDataType_U8, &m_iv);

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("(0 - 255)");

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Pack type");
    ImGui::Unindent(3.0f);

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::RadioButton("any##Filter", &m_filter, 0);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Encrypt all files from input");

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("client##Filter", &m_filter, 1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Encrypt only client files");

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("server##Filter", &m_filter, 2);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Encrypt only server files");

    ImGui::Dummy({ 0.0f, 10.0f });

    if (m_components.view.is_working)
        ImGui::EndDisabled();

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImVec2 avail = ImGui::GetContentRegionMax();
    ImGui::SetCursorPosX((avail.x) / 2 - (125 / 2.0f));

    bool cannot_start = !can_start();

    if (cannot_start)
        ImGui::BeginDisabled();

    if (!m_components.view.is_working) {
        if (ImGui::Button("Encrypt##Btn", { 125, 20 })) {
            m_components.systems.crypt.set_start_callback([&]() { handle_on_start(); });
            m_components.systems.crypt.set_finish_callback([&](bool success) { handle_on_finish(success); });
            m_components.systems.crypt.set_update_callback([&](float value) { handle_on_update(value); });
            m_components.systems.crypt.set_error_callback([&](const std::string& msg) { handle_on_error(msg); });

            m_components.systems.crypt.set_cancel_token(&m_cancel);

            m_components.systems.crypt.encrypt(m_input, m_output, m_filter, m_key, m_iv);
        }
    }
    else {
        if (ImGui::Button("Cancel##Btn", { 125, 20 })) {
            m_cancel = true;
        }
    }

    if (cannot_start)
        ImGui::EndDisabled();

    if (m_components.view.is_working)
        ImGui::BeginDisabled();

    ImGui::Unindent(20.0f);

    ImGui::PopID();
}

bool comp_view_encrypt::can_start() {
    bool result = !m_input.empty() && !m_key.empty();

    if (m_encrypt_to_dir)
        result = result && !m_output.empty();

    return result;
}
