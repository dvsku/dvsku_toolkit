#include "gui/dt_comp_root.hpp"
#include "dvsku_toolkit.hpp"
#include "dt_app.hpp"

#include <dv_gui_opengl\fonts\dv_font_fontawesome_solid.hpp>

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_comp_root::dt_comp_root(dt_app& app)
    : dt_gui_base(app) {}

void dt_comp_root::render() {
    std::lock_guard<std::mutex> guard(m_app.get_systems().core.mutex);

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoNav;
    flags |= ImGuiWindowFlags_NoDecoration;
    flags |= ImGuiWindowFlags_NoSavedSettings;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoDocking;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImRect rectangle = { {0.0f, 0.0f}, {0.0f, 0.0f} };

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    if (ImGui::Begin("Root##Component", NULL, flags)) {
        ImGuiWindowFlags cflags = 0;
        cflags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

        ///////////////////////////////////////////////////////////////////////////
        // TITLE BAR

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f));
        if (ImGui::BeginChild("##TitleBar", { 0.0f, 25.0f }, false, cflags)) {
            ImGui::PopStyleColor(1);

            auto max = ImGui::GetWindowContentRegionMax();
            auto min = ImGui::GetWindowContentRegionMin();

            ImGui::Dummy({ 0.0f, 2.0f });
            ImGui::Indent(10.0f);

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.77255f, 0.77255f, 0.77255f, 1.00f));
            ImGui::Text("%s v%s", DT_APP_NAME, m_app.get_systems().core.app_version.to_string().c_str());
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Button, 0x00FFFFFF);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xFF3D3D3D);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xFF3D3D3D);
            ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);

            ImGui::SameLine(0.0f, 0.0f);

            ImGui::SetCursorPosX(max.x - 60.0f);
            ImGui::SetCursorPosY(min.y);

            if (ImGui::Button(ICON_FA_MINUS"##Minimize", { 30.0f, 25.0f })) {
                m_app.minimize();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
            ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
            ImGui::SetItemTooltip("Minimize");
            ImGui::PopStyleColor(1);
            ImGui::PopStyleVar(1);

            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(min.y);
            if (ImGui::Button(ICON_FA_XMARK"##Quit", { 30.0f, 25.0f })) {
                m_app.close();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
            ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
            ImGui::SetItemTooltip("Close");
            ImGui::PopStyleColor(1);
            ImGui::PopStyleVar(1);

            ImGui::PopStyleColor(4);

            ImGui::Unindent(10.0f);

            rectangle = ImGui::GetCurrentWindow()->Rect();
        }
        ImGui::EndChild();

        m_app.m_is_title_bar = ImGui::IsMouseHoveringRect(rectangle.Min, rectangle.Max);

        ///////////////////////////////////////////////////////////////////////////
        // CONTENT

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.0f);

        if (ImGui::BeginChild("##Content", { -15.0f, -25.0f }, false, cflags)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 6.0f, 4.0f });
            ImGui::Indent(15.0f);

            bool is_working = m_app.get_systems().core.is_working;

            if (is_working)
                ImGui::BeginDisabled();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
            if (ImGui::BeginTabBar("TabViews")) {
                if (ImGui::BeginTabItem("Pack##TabView")) {
                    m_app.get_gui().pack.render();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Unpack##TabView")) {
                    m_app.get_gui().unpack.render();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            if (is_working)
                ImGui::EndDisabled();

            ImGui::Unindent(10.0f);
            ImGui::PopStyleVar(1);
        }
        ImGui::EndChild();

        ///////////////////////////////////////////////////////////////////////////
        // FOOTER

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.0f);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f));
        if (ImGui::BeginChild("##Footer", { 0.0f, 25.0f }, false)) {
            ImGui::Indent(8.0f);
            
            bool show_errors = m_app.get_systems().core.has_errors;

            if (show_errors) {
                ImGui::PushStyleColor(ImGuiCol_Button,        0x00000000);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0x00000000);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,  0x00000000);
                ImGui::PushStyleColor(ImGuiCol_Text,          IM_COL32(224, 29, 33, 255));

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
                if (ImGui::Button(ICON_FA_TRIANGLE_EXCLAMATION"##ErrorButton", { 19.0f, 19.0f })) {
                    m_app.get_systems().command.set_to_execute(dt_commands::flag_show_error_window);
                }

                ImGui::PopStyleColor(4);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.0f, 6.0f });
                ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);
                ImGui::SetItemTooltip("Click to see errors");
                ImGui::PopStyleColor(1);
                ImGui::PopStyleVar(1);
            }

            ImGui::Unindent(8.0f);
        }
        ImGui::PopStyleColor(1);
        ImGui::EndChild();

    }
    ImGui::PopStyleVar(1);
    ImGui::End();

    m_app.get_systems().command.execute_all();
}
