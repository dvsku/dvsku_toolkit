#pragma once

#include "systems/dt_system_base.hpp"

#include <libevp.hpp>

namespace dvsku_toolkit {
    class dt_app;

    class dt_sys_evp : public dt_system_base {
    public:
        dt_sys_evp()                  = delete;
        dt_sys_evp(const dt_sys_evp&) = delete;
        dt_sys_evp(dt_sys_evp&&)      = delete;

        dt_sys_evp& operator=(const dt_sys_evp&) = delete;
        dt_sys_evp& operator=(dt_sys_evp&&)      = delete;

        dt_sys_evp(dt_app& app)
            : dt_system_base(app) {};

    public:
        void pack(const std::string& input, const std::string& output, int filter, libevp::evp_context* context = nullptr);
        void unpack(const std::string& input, const std::string& output, libevp::evp_context* context = nullptr);

    private:
        libevp::evp m_evp;
    };
}