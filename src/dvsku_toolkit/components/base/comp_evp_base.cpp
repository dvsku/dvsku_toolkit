#include "components/base/comp_evp_base.hpp"
#include "components/components_bundle.hpp"
#include "systems/systems_bundle.hpp"

using namespace dvsku_toolkit;

comp_evp_base::comp_evp_base(components_bundle& components)
    : m_components(components) {}

void comp_evp_base::handle_on_start() {
    m_components.view.is_working = true;

    m_components.systems.taskbar.set_status(TBPF_NORMAL);
    m_components.systems.taskbar.set_progress(0.0f);

    m_progress = 0.0f;
    sprintf(m_progress_text, "%.2f%c", m_progress, '%');
}

void comp_evp_base::handle_on_finish(bool success) {
    if (success) {
        m_progress = 100.0f;
        m_input    = "";
        m_output   = "";
        m_key      = "";

        sprintf(m_progress_text, "%.2f%c", m_progress, '%');
    }
    else {
        m_progress = 0.0f;

        sprintf(m_progress_text, "Cancelled");
    }

    m_cancel                     = false;
    m_components.view.is_working = false;

    m_components.systems.taskbar.set_status(TBPF_NOPROGRESS);
    m_components.systems.taskbar.set_progress(0.0f);

    m_components.systems.sound.play_success_sound();
}

void comp_evp_base::handle_on_update(float progress) {
    m_progress += progress;
    sprintf(m_progress_text, "%.2f%c", m_progress, '%');

    m_components.systems.taskbar.set_progress(m_progress);
}

void comp_evp_base::handle_on_error(const std::string& msg) {
    m_components.view.is_working = false;

    m_components.systems.taskbar.set_status(TBPF_ERROR);
    m_components.systems.taskbar.set_progress(0.0f);

    m_components.systems.sound.play_error_sound();
}
