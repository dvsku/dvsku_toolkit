#include "systems/dt_sys_command.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;
using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

void dt_sys_command::prepare() {
    set_command(dt_commands::flag_show_error_window, dv_command([this]() {
        return m_app.get_gui().errors.render();
    }));
}

void dt_sys_command::release() {

}
