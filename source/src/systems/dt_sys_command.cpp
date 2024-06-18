#include "systems/dt_sys_command.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;
using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

void dt_sys_command::prepare() {
    set_command(dt_commands::flag_show_error_window, command([this]() {
        return m_app.get_gui().modal_errors.render();
    }));
}

void dt_sys_command::release() {

}
