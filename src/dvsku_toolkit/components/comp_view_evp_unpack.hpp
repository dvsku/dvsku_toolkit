#pragma once

#include "components/base/comp_evp_base.hpp"

namespace dvsku_toolkit {
    class comp_view_evp_unpack : public comp_evp_base {
    public:
        comp_view_evp_unpack(components_bundle& components);

        void render() override;

    private:
        bool m_decrypt = false;

    private:
        bool can_start() override;
    };
}



