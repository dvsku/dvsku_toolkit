#pragma once

#include "libdvsku_crypt/libdvsku_crypt.hpp"

#include <string>

namespace dvsku_toolkit {
    class components_bundle;

    class comp_crypt_base {
    public:
        comp_crypt_base(components_bundle& components);

    public:
        virtual void render() = 0;

    protected:
        components_bundle& m_components;

        bool m_cancel = false;

        std::string m_input  = "";
        std::string m_output = "";
        std::string m_key    = "";

        uint8_t m_iv = 0U;

        float m_progress          = 0.0f;
        char  m_progress_text[12] = "";

    protected:
        virtual bool can_start() = 0;
    };
}
