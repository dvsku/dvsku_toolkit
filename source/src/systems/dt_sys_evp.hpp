#pragma once

#include "systems/dt_system_base.hpp"

#include <libevp.hpp>

namespace dvsku_toolkit {
    class dt_app;

    class dt_sys_evp : public dt_system_base {
    public:
        dt_sys_evp(dt_app& app);

    public:
        void pack(const std::string& input, const std::string& output, int filter);
        void unpack(const std::string& input, const std::string& output);

    private:
        libevp::evp         m_evp;
        libevp::evp_context m_evp_context;
    };
}
