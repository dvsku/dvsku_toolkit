#include "components/comp_view_evp_pack.hpp"
#include "components/components_bundle.hpp"
#include "systems/systems_bundle.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "utilities/utilities_file_dialog.hpp"

using namespace dvsku_toolkit;

comp_view_evp_pack::comp_view_evp_pack(components_bundle& components) 
    : comp_evp_base(components) {}

void comp_view_evp_pack::render() {
    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 15.0f, 15.0f });
    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF2E2E2E);

    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##InputPack", &m_input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##InputPack", { 125, 20 })) {
        file_dialog::select_folder(m_input);
    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##OutputPack", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##OutputPack", { 125, 20 })) {
        file_dialog::save_file(m_output);
    }

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::Checkbox("##PackEncrypt", &m_encrypt);
    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("Encrypt");

    ImGui::Dummy({ 0.0f, 3.0f });

    if (!m_encrypt)
        ImGui::BeginDisabled();

    ImGui::Indent(3.0f);
    ImGui::Text("Key");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::InputText("##PackKey", &m_key);

    ImGui::Indent(3.0f);
    ImGui::Text("IV");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(120.0f);
    ImGui::InputScalar("##PackIv", ImGuiDataType_U8, &m_iv);

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::Text("(0 - 255)");

    if (!m_encrypt)
        ImGui::EndDisabled();

    //ImGui::PushStyleColor(ImGuiCol_PlotHistogram, 0xFF774F2D);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Pack type");
    ImGui::Unindent(3.0f);

    ImGui::Dummy({ 0.0f, 3.0f });

    ImGui::RadioButton("any##PackFilter", &m_filter, 0);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Pack all files from input");

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("client##PackFilter", &m_filter, 1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Pack only client files");

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::RadioButton("server##PackFilter", &m_filter, 2);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Pack only server files");

    ImGui::Dummy({ 0.0f, 10.0f });

    if (m_cancel)
        strcpy(m_progress_text, "Cancelled");
    else
        sprintf(m_progress_text, "%.2f%c", m_progress, '%');

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_progress + 0.5f, ImVec2(0.0f, 0.0f), m_progress_text);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImVec2 avail = ImGui::GetContentRegionAvail();
    float new_x = avail.x - ImGui::GetCursorPosX();

    ImGui::SetCursorPosX((avail.x + 20.0f) / 2 - (125 / 2.0f));

    if (!can_start())
        ImGui::BeginDisabled();

    if (ImGui::Button("Pack##Pack", { 125, 20 })) {
        m_components.systems.evp.set_start_callback([&]() { handle_on_start(); });
        m_components.systems.evp.set_finish_callback([&](bool success) { handle_on_finish(success); });
        m_components.systems.evp.set_update_callback([&](float value) { handle_on_update(value); });
        m_components.systems.evp.set_error_callback([&](const std::string& msg) { handle_on_error(msg); });

        m_components.systems.evp.pack(m_input, m_output, m_filter, m_encrypt, m_key, m_iv);
    }

    if (!can_start())
        ImGui::EndDisabled();

    //ImGui::PopStyleColor();

    ImGui::Unindent(20.0f);

    //ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

bool comp_view_evp_pack::can_start() {
    bool result = !m_input.empty() && !m_output.empty();

    if (m_encrypt)
        result = result && !m_key.empty();

    return result;
}
