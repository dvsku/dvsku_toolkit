#include "components/comp_view_evp_unpack.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

using namespace dvsku_toolkit;

comp_view_evp_unpack::comp_view_evp_unpack(components_bundle& components) 
    : comp_evp_base(components) {}

void comp_view_evp_unpack::render() {
    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 15.0f, 15.0f });
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

    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##OutputUnpack", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##OutputUnpack", { 125, 20 })) {

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

    //ImGui::PushStyleColor(ImGuiCol_PlotHistogram, 0xFF774F2D);

    ImGui::Dummy({ 0.0f, 68.0f });

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

    if (ImGui::Button("Unpack##Unpack", { 125, 20 })) {

    }

    if (!can_start())
        ImGui::EndDisabled();

    //ImGui::PopStyleColor();

    ImGui::Unindent(20.0f);

    //ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

bool comp_view_evp_unpack::can_start() {
    bool result = !m_input.empty() && !m_output.empty();

    if (m_decrypt)
        result = result && !m_key.empty();

    return result;
}
