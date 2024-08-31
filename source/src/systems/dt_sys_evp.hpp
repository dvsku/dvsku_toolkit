#pragma once

#include "systems/dt_system_base.hpp"
#include "models/pack_input.hpp"
#include "models/unpack_input.hpp"

#include <libevp.hpp>
#include <filesystem>
#include <vector>

namespace dvsku_toolkit {
    class dt_app;

    class dt_sys_evp : public dt_system_base {
    public:
        pack_input   packing_inputs   = {};
        unpack_input unpacking_inputs = {};

    public:
        dt_sys_evp(dt_app& app);

    public:
        void pack(const std::string& input, const std::string& output, int filter);
        void unpack(const std::string& input, const std::string& output);
        void set_pack_files();
        void set_unpack_files();

    private:
        libevp::evp         m_evp;
        libevp::evp_context m_evp_context;

    private:
        std::vector<std::filesystem::path> get_pack_files();
    };
}
