#include "systems/dt_system_mngr.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_system_mngr::dt_system_mngr(dt_app& app)
    : core(app), evp(app), command(app) {}
