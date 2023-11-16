#pragma once

#include "components/base/comp_crypt_base.hpp"

namespace dvsku_toolkit {
    class comp_view_encrypt : public comp_crypt_base {
    public:
        comp_view_encrypt(components_bundle& components);

        void render() override;

    private:
        bool m_encrypt_to_dir = false;
        int  m_filter         = 0;

    private:
        bool can_start() override;
    };
}