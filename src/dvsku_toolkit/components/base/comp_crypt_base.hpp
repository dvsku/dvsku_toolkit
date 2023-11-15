#pragma once

#include "libdvsku_crypt/libdvsku_crypt.hpp"

#include <string>

namespace dvsku_toolkit {
    class components_bundle;

    class comp_crypt_base {
    public:
        comp_crypt_base(components_bundle& components);

    protected:
        components_bundle& m_components;

        bool m_cancel = false;

        std::string m_input  = "";
        std::string m_output = "";
        std::string m_key    = "";

        float m_progress = 0.0f;
    };
}
