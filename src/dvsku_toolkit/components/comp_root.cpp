#include "components/comp_root.hpp"
#include "components/components_bundle.hpp"

#include "imgui.h"
#include "fonts/font_fontawesome_solid.hpp"

using namespace dvsku_toolkit;

comp_root::comp_root(components_bundle& components)
    : m_components(components), m_view_pack(components), m_view_unpack(components),
      m_view_encrypt(components), m_view_decrypt(components) {}

void comp_root::render() {
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

    if (is_working)
        ImGui::EndDisabled();
}
