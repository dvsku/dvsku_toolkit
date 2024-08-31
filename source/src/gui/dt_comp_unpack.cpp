#include "gui/dt_comp_unpack.hpp"
#include "dt_app.hpp"

#include <libgui.hpp>
#include <stack>

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

void dt_comp_unpack::render() {
    auto max = ImGui::GetContentRegionMax();

    ImGui::PushID("Unpack");
    ImGui::Indent(20.0f);

    ImGui::Dummy({ 0.0f, 5.0f });

    ImGui::Indent(3.0f);
    ImGui::Text("Input");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-50.0f);
    ImGui::InputText("##Input", &m_app.systems.evp.unpacking_inputs.input, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (libgui::imgui::icon_button(ICON_FA_FILE"##InputSelect", { 25.0f, 21.0f })) {
        m_app.systems.evp.unpacking_inputs.input =
            libgui::dialog::open_file("Select input file", m_app.systems.evp.unpacking_inputs.input, { "EVP (*.evp)", "*.evp" });
        
        m_app.systems.evp.set_unpack_files();
    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-50.0f);
    ImGui::InputText("##Output", &m_app.systems.evp.unpacking_inputs.output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (libgui::imgui::icon_button(ICON_FA_FOLDER"##OutputSelect", { 25.0f, 21.0f })) {
        m_app.systems.evp.unpacking_inputs.output =
            libgui::dialog::select_dir("Select output directory", m_app.systems.evp.unpacking_inputs.output);
    }

    ImGui::Dummy({ 0.0f, 3.0f });

    libgui::theme::push_col(ImGuiCol_ChildBg,     libgui::fRGBA_to_RGBA(0.12157f, 0.12157f, 0.12157f, 1.00f));
    libgui::theme::push_col(ImGuiCol_ScrollbarBg, libgui::fRGBA_to_RGBA(0.12157f, 0.12157f, 0.12157f, 1.00f));

    if (ImGui::BeginChild("##Files", { -20.0f, -68.0f })) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });
        libgui::theme::push_col(ImGuiCol_ChildBg, libgui::fRGBA_to_RGBA(0.10f, 0.10f, 0.10f, 1.00f));

        if (ImGui::BeginChild("##Search", { 0.0f, 31.0f }, false, ImGuiWindowFlags_AlwaysUseWindowPadding)) {
            ImGuiInputTextFlags search_flags = 0;
            search_flags |= ImGuiInputTextFlags_EnterReturnsTrue;
            search_flags |= ImGuiInputTextFlags_AutoSelectAll;

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##SearchInput", &m_app.systems.evp.unpacking_inputs.search_query, search_flags)) {
                m_app.systems.evp.unpacking_inputs.tree.search(m_app.systems.evp.unpacking_inputs.search_query);
            }
        }
        ImGui::EndChild();

        libgui::theme::pop_col(1);
        ImGui::PopStyleVar();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.0f, 5.0f });

        if (ImGui::BeginChild("##Tree", { 0.0f, 0.0f }, false, ImGuiWindowFlags_AlwaysUseWindowPadding)) {
            render_tree();
        }
        ImGui::EndChild();

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    libgui::theme::pop_col(2);

    bool is_working = m_app.systems.core.work_context.is_working();

    if (is_working)
        ImGui::EndDisabled();

    ImGui::SetCursorPosY(max.y - 58.0f);

    ImGui::SetNextItemWidth(-20.0f);
    ImGui::ProgressBar(m_app.systems.core.work_context.get_progress() / 100, ImVec2(0.0f, 0.0f));

    auto avail        = ImGui::GetContentRegionMax();
    bool cannot_start = m_app.systems.evp.unpacking_inputs.input.empty()  ||
                        m_app.systems.evp.unpacking_inputs.output.empty() ||
                        m_app.systems.evp.unpacking_inputs.tree.root->get_check_state() == visual_tree_branch::check_state::unchecked;;

    ImGui::SetCursorPosX((avail.x) / 2 - (125 / 2.0f));
    ImGui::SetCursorPosY(max.y - 29.0f);
    
    if (cannot_start)
        ImGui::BeginDisabled();

    if (!is_working) {
        if (ImGui::Button("Unpack##Unpack", { 125.0f, 21.0f })) {
            m_app.systems.evp.unpack();
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

void dt_comp_unpack::render_tree() {
    struct stack_element {
        std::shared_ptr<visual_tree_element> ptr = nullptr;
        int                                  level = 0;
    };

    std::stack<stack_element> stack;

    for (auto it = m_app.systems.evp.unpacking_inputs.tree.root->leaves.rbegin(); it != m_app.systems.evp.unpacking_inputs.tree.root->leaves.rend(); it++) {
        stack.push({ *it, 0 });
    }

    for (auto it = m_app.systems.evp.unpacking_inputs.tree.root->branches.rbegin(); it != m_app.systems.evp.unpacking_inputs.tree.root->branches.rend(); it++) {
        stack.push({ *it, 0 });
    }

    while (stack.size() != 0) {
        stack_element element = stack.top();
        stack.pop();

        if (!element.ptr)
            continue;

        if (element.ptr->type == visual_tree_element::element_type::branch) {
            visual_tree::branch_ptr_t branch = std::static_pointer_cast<visual_tree_branch>(element.ptr);

            if (m_app.systems.evp.unpacking_inputs.tree.is_hidden_by_search(branch))
                continue;

            ImGui::Indent(1.0f + element.level * 15.0f);

            ImGui::PushID("Branch");
            ImGui::PushID(branch.get());

            libgui::theme::push_col(ImGuiCol_FrameBg, libgui::fRGBA_to_RGBA(0.10f, 0.10f, 0.10f, 1.00f));

            bool checkbox_pressed = false;
            bool is_checked = false;

            visual_tree_branch::check_state check_state = branch->get_check_state();
            if (check_state == visual_tree_branch::check_state::intermediate) {
                ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                checkbox_pressed = ImGui::Checkbox("##Checkbox", &is_checked);
                ImGui::PopItemFlag();
            }
            else {
                is_checked = check_state == visual_tree_branch::check_state::checked;
                checkbox_pressed = ImGui::Checkbox("##Checkbox", &is_checked);
            }

            if (checkbox_pressed) {
                branch->check();
            }

            libgui::theme::pop_col();

            ImGui::SameLine(0.0f, 21.0f);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 1.0f, 4.0f });
            bool expanded = ImGui::CollapsingHeader(DV_FORMAT(ICON_FA_FOLDER_CLOSED " {}###Dir", branch->name.c_str()).c_str());
            ImGui::PopStyleVar();
            if (expanded) {
                for (auto it = branch->leaves.rbegin(); it != branch->leaves.rend(); it++) {
                    stack.push({ *it, element.level + 1 });
                }

                for (auto it = branch->branches.rbegin(); it != branch->branches.rend(); it++) {
                    stack.push({ *it, element.level + 1 });
                }
            }

            ImGui::PopID();
            ImGui::PopID();

            ImGui::Unindent(1.0f + element.level * 15.0f);
        }
        else {
            visual_tree::leaf_ptr_t leaf = std::static_pointer_cast<visual_tree_leaf>(element.ptr);

            if (m_app.systems.evp.unpacking_inputs.tree.is_hidden_by_search(leaf))
                continue;

            ImGui::Indent(1.0f + element.level * 15.0f);

            ImGui::PushID("Leaf");
            ImGui::PushID(leaf.get());

            libgui::theme::push_col(ImGuiCol_FrameBg, libgui::fRGBA_to_RGBA(0.10f, 0.10f, 0.10f, 1.00f));

            bool is_checked = leaf->is_checked();
            if (ImGui::Checkbox(DV_FORMAT(" " ICON_FA_FILE " {}##Checkbox", leaf->name).c_str(), &is_checked)) {
                leaf->check();
            }

            libgui::theme::pop_col();

            ImGui::PopID();
            ImGui::PopID();

            ImGui::Unindent(1.0f + element.level * 15.0f);
        }
    }
}
