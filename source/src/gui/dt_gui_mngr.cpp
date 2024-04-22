#include "gui/dt_gui_mngr.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_gui_mngr::dt_gui_mngr(dt_app& app)
    : root(app), pack(app), unpack(app) {}
