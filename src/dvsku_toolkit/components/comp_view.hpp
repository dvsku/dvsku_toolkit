#pragma once

#include "components/comp_view_evp_pack.hpp"
#include "components/comp_view_evp_unpack.hpp"
#include "components/comp_view_encrypt.hpp"
#include "components/comp_view_decrypt.hpp"

namespace dvsku_toolkit {
    class components_bundle;

    class comp_view {
    public:
        bool is_working = false;

    public:
        comp_view(components_bundle& components);

        void render();

    private:
        components_bundle& m_components;

        comp_view_evp_pack   m_view_pack;
        comp_view_evp_unpack m_view_unpack;
        comp_view_encrypt    m_view_encrypt;
        comp_view_decrypt    m_view_decrypt;
    };
}
