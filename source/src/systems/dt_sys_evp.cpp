#include "systems/dt_sys_evp.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

void dt_sys_evp::pack(const std::string& input, const std::string& output, int filter, libevp::evp_context* context) {
    m_evp.pack_async(input, output, (libevp::evp_filter)filter, context);
}

void dt_sys_evp::unpack(const std::string& input, const std::string& output, libevp::evp_context* context) {
    m_evp.unpack_async(input, output, context);
}
