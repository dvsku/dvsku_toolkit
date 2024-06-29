#include "systems/dt_sys_evp.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_sys_evp::dt_sys_evp(dt_app& app)
    : dt_system_base(app)
{
    m_evp_context.cancel = &m_app.systems.core.work_context.cancel;

    m_evp_context.update_callback = [this](float progress_change) {
        m_app.systems.core.work_context.update_progress(progress_change);
    };

    m_evp_context.finish_callback = [this](libevp::evp_result result) {
        bool success = result.status == libevp::evp_result_status::ok ||
            result.status == libevp::evp_result_status::cancelled;

        if (result.status == libevp::evp_result_status::error && result.message != "") {
            std::lock_guard guard(m_app.systems.core.mutex);
            m_app.systems.core.errors = result.message;
        }

        m_app.systems.core.work_context.stop_working(success);
        m_app.systems.core.work_context.set_progress(0.0f);
    };
};

void dt_sys_evp::pack(const std::string& input, const std::string& output, int filter) {
    if (m_app.systems.core.work_context.is_working())
        return;
    
    m_app.systems.core.errors = "";

    m_app.systems.core.work_context.start_working();
    m_evp.pack_async(input, output, (libevp::evp_filter)filter, &m_evp_context);
}

void dt_sys_evp::unpack(const std::string& input, const std::string& output) {
    if (m_app.systems.core.work_context.is_working())
        return;

    m_app.systems.core.errors = "";

    m_app.systems.core.work_context.start_working();
    m_evp.unpack_async(input, output, &m_evp_context);
}
