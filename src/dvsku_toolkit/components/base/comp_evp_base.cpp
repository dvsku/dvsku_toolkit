#include "components/base/comp_evp_base.hpp"
#include "components/components_bundle.hpp"

using namespace dvsku_toolkit;

comp_evp_base::comp_evp_base(components_bundle& components)
    : m_components(components) {}

void comp_evp_base::handle_on_start() {
    m_components.view.is_working = true;

    //GUI.set_disabled(true);
    //GUI.set_taskbar_status(TBPF_NORMAL);
    //GUI.set_taskbar_progress(0.0f);
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

    //GUI.set_disabled(false);
    //GUI.set_taskbar_status(TBPF_NOPROGRESS);
    //GUI.set_taskbar_progress(0.0f);
    //PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}

void comp_evp_base::handle_on_update(float progress) {
    m_progress += progress;
    sprintf(m_progress_text, "%.2f%c", m_progress, '%');
    /*GUI.set_taskbar_progress(m_progress);*/
}

void comp_evp_base::handle_on_error(const std::string& msg) {
    /*GUI.set_disabled(false);
    GUI.set_taskbar_status(TBPF_ERROR);
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);*/
}
