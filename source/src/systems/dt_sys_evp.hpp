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
        void pack();
        void unpack();

        void set_pack_files();
        void set_unpack_files();

    private:
        enum class work_type : uint8_t {
            none,
            packing,
            unpacking
        };

    private:
        libevp::evp         m_evp;
        libevp::evp_context m_evp_context;
        work_type           m_current_work = work_type::none;

    private:
        std::vector<std::filesystem::path> get_pack_files();
    };
}
