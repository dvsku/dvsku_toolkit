#include "components/comp_view_encrypt.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "utilities/utilities_file_dialog.hpp"

using namespace dvsku_toolkit;

comp_view_encrypt::comp_view_encrypt(components_bundle& components) 
    : comp_crypt_base(components) {}

void comp_view_encrypt::render() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF2E2E2E);

    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##InputEncrypt", &m_input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##InputEncrypt", { 125, 20 })) {
        file_dialog::select_folder(m_input);
    }

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::Checkbox("##EncryptToDir", &m_encrypt_to_dir);
    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("Encrypt to folder");

    ImGui::Dummy({ 0.0f, 3.0f });

    if (!m_encrypt_to_dir)
        ImGui::BeginDisabled();

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##OutputEncrypt", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##OutputEncrypt", { 125, 20 })) {
        file_dialog::select_folder(m_output);
    }

    if (!m_encrypt_to_dir)
        ImGui::EndDisabled();

    ImGui::Indent(3.0f);
    ImGui::Text("Key");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::InputText("##EncryptKey", &m_key);

    ImGui::Indent(3.0f);
    ImGui::Text("IV");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(120.0f);
    ImGui::InputScalar("##EncryptIv", ImGuiDataType_U8, &m_iv);

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("(0 - 255)");

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Pack type");
    ImGui::Unindent(3.0f);

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::RadioButton("any##EncryptFilter", &m_filter, 0);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Encrypt all files from input");

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("client##EncryptFilter", &m_filter, 1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Encrypt only client files");

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("server##EncryptFilter", &m_filter, 2);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Encrypt only server files");

    ImGui::Dummy({ 0.0f, 10.0f });

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImVec2 avail = ImGui::GetContentRegionMax();
    ImGui::SetCursorPosX((avail.x) / 2 - (125 / 2.0f));

    if (!can_start())
        ImGui::BeginDisabled();

    if (ImGui::Button("Encrypt##Encrypt", { 125, 20 })) {

    }

    if (!can_start())
        ImGui::EndDisabled();

    ImGui::Unindent(20.0f);

    ImGui::PopStyleColor();
}

bool comp_view_encrypt::can_start() {
    bool result = !m_input.empty() && !m_key.empty();

    if (m_encrypt_to_dir)
        result = result && !m_output.empty();

    return result;
}
