#pragma once

#include "components/comp_view_evp_pack.hpp"
#include "components/comp_view_evp_unpack.hpp"
#include "components/comp_view_encrypt.hpp"
#include "components/comp_view_decrypt.hpp"

namespace dvsku_toolkit {
    class components_bundle;

    class comp_root {
    public:
        comp_root(components_bundle& components);

        void render();

    private:
        components_bundle& m_components;

        comp_view_evp_pack   m_view_pack;
        comp_view_evp_unpack m_view_unpack;
        comp_view_encrypt    m_view_encrypt;
        comp_view_decrypt    m_view_decrypt;

        bool m_hovered_minimize = false;
        bool m_hovered_close    = false;
        bool m_active_minimize  = false;
        bool m_active_close     = false;
    };
}
