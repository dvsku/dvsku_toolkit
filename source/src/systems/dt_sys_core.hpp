#pragma once

#include "systems/dt_system_base.hpp"

#include <libgui.hpp>
#include <libgui/misc/work_context.hpp>
#include <mutex>

namespace dvsku_toolkit {
    class dt_app;

    class dt_sys_core : public dt_system_base {
    public:
        libgui::version      app_version;
        libgui::work_context work_context;
        std::string          errors = "";
        std::mutex           mutex;

    public:
        dt_sys_core(dt_app& app);
    };
}
