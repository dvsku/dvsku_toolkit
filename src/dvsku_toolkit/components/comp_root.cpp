#include "components/comp_root.hpp"
#include "components/components_bundle.hpp"

#include "imgui.h"

using namespace dvsku_toolkit;

comp_root::comp_root(components_bundle& components)
    : m_components(components), m_view_pack(components), m_view_unpack(components),
      m_view_encrypt(components), m_view_decrypt(components) {}

void comp_root::render() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, 0xFF1F1F1F);

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Root##Window", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

   //auto atlas = ImGui::GetIO().Fonts;

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

    ImGui::BeginChild("TitleBar", { 0.0f, 25.0f });

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);
    ImGui::Text("dvsku toolkit");

    ImGui::SameLine();

    ImVec2 avail = ImGui::GetContentRegionAvail();

    ImGui::PushStyleColor(ImGuiCol_Button,        0x00FFFFFF);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0x00FFFFFF);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  0x00FFFFFF);

    if (!m_hovered_minimize && !m_active_minimize)
        ImGui::PushStyleColor(ImGuiCol_Text, 0x88FFFFFF);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFFFFFFF);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + avail.x - 45.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
    if (ImGui::Button("-##MinimizeButton", { 20.0f, 15.0f })) {

    }

    m_hovered_minimize = ImGui::IsItemHovered();
    m_active_minimize  = ImGui::IsItemActive();

    ImGui::SameLine(0.0f, 5.0f);

    if (!m_hovered_close && !m_active_close)
        ImGui::PushStyleColor(ImGuiCol_Text, 0x88FFFFFF);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFFFFFFF);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
    if (ImGui::Button("X##CloseButton", { 20.0f, 15.0f })) {

    }

    m_hovered_close = ImGui::IsItemHovered();
    m_active_close  = ImGui::IsItemActive();

    ImGui::PopStyleColor(5);

    ImGui::EndChild();

    ImGui::PopFont();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.0f, 0.0f });

    ImGui::BeginChild("View", { 0.0f , 0.0f}, false, ImGuiWindowFlags_AlwaysUseWindowPadding);

    if (is_working)
        ImGui::BeginDisabled();

    if (ImGui::BeginTabBar("ViewTabs")) {
        if (ImGui::BeginTabItem("Pack##ViewTab")) {
            m_view_pack.render();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Unpack##ViewTab")) {
            m_view_unpack.render();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Encrypt##ViewTab")) {
            m_view_encrypt.render();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Decrypt##ViewTab")) {
            m_view_decrypt.render();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::PopStyleVar();

    if (is_working)
        ImGui::EndDisabled();

    ImGui::EndChild();

    ImGui::End();

    ImGui::PopStyleColor();
}
