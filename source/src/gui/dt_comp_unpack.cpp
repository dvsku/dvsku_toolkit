#include "gui/dt_comp_unpack.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_comp_unpack::dt_comp_unpack(dt_app& app)
    : dt_gui_base(app)
{
    m_context.start_callback = [this]() {
        std::lock_guard<std::mutex> guard(m_app.get_systems().core.mutex);

        m_progress                          = 0.0f;
        m_app.get_systems().core.is_working = true;
        
        m_app.set_taskbar_status(dvsku::gui_taskbar_status::normal);
        m_app.set_taskbar_progress(0U);
    };
    m_context.finish_callback = [this](libevp::evp_result result) {
        std::lock_guard<std::mutex> guard(m_app.get_systems().core.mutex);

        m_progress                          = 100.0f;
        m_app.get_systems().core.is_working = false;
        m_app.get_systems().core.has_errors = false;
        m_app.get_systems().core.errors     = "";

        m_app.set_taskbar_status(dvsku::gui_taskbar_status::no_progress);
        m_app.set_taskbar_progress(0U);

        if (result.status == libevp::evp_result_status::ok) {
            dvsku::util_sound::success();
        }
        else if (result.status == libevp::evp_result_status::error) {
            dvsku::util_sound::warning();

            if (!result.message.empty()) {
                m_app.get_systems().core.has_errors = true;
                m_app.get_systems().core.errors     = result.message;
            }
        }
    };
    m_context.update_callback = [this](float progress) {
        std::lock_guard<std::mutex> guard(m_app.get_systems().core.mutex);

        m_progress += progress;
        m_app.set_taskbar_progress((uint64_t)m_progress);
    };
    m_context.cancel = &m_cancel;
}

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
        m_input = dvsku::util_dialog::open_file("Input evp", m_input, { "EVP (*.evp)", "*.evp" });
    }

    ImGui::Indent(3.0f);
    ImGui::Text("Output");
    ImGui::Unindent(3.0f);

    ImGui::SetNextItemWidth(-150.0f);
    ImGui::InputText("##Output", &m_output, ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button("Select##Output", { 125.0f, 21.0f })) {
        m_output = dvsku::util_dialog::select_dir("Output dir", m_output);
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

    bool cannot_start = m_input.empty() || m_output.empty() || m_cancel;

    if (cannot_start)
        ImGui::BeginDisabled();

    if (!is_working) {
        if (ImGui::Button("Unpack##Unpack", { 125.0f, 21.0f })) {
            m_app.get_systems().evp.unpack(m_input, m_output, &m_context);
        }
    }
    else {
        if (ImGui::Button("Cancel##Cancel", { 125.0f, 21.0f })) {
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
