#include "systems/dt_sys_core.hpp"
#include "dt_app.hpp"
#include "dvsku_toolkit.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_sys_core::dt_sys_core(dt_app& app)
    : dt_system_base(app),
      work_context(static_cast<libgui::window&>(app))
{
    app_version = libgui::version(DT_VERSION_MAJOR, DT_VERSION_MINOR, DT_VERSION_REV);
};