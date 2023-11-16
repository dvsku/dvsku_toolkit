#pragma once

#include "components/base/comp_crypt_base.hpp"

namespace dvsku_toolkit {
    class comp_view_decrypt : public comp_crypt_base {
    public:
        comp_view_decrypt(components_bundle& components);

        void render() override;

    private:
        bool m_decrypt_to_dir = false;

    private:
        bool can_start() override;
    };
}
