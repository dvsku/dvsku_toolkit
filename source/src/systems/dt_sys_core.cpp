#include "systems/dt_sys_core.hpp"
#include "dvsku_toolkit.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_sys_core::dt_sys_core(dt_app& app)
    : dt_system_base(app)
{
    app_version = dvsku::dv_version(DT_VERSION_MAJOR, DT_VERSION_MINOR, DT_VERSION_REV);
};