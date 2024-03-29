#pragma once

#include "components/base/comp_evp_base.hpp"

namespace dvsku_toolkit {
    class comp_view_evp_pack : public comp_evp_base {
    public:
        comp_view_evp_pack(components_bundle& components);

        void render() override;

    private:
        bool m_encrypt = false;
        int  m_filter  = 0;

    private:
        bool can_start() override;
    };
}
