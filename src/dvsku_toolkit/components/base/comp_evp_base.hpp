#pragma once

#include "libevp/libevp.hpp"

#include <string>

namespace dvsku_toolkit {
    class components_bundle;

    class comp_evp_base {
    public:
        comp_evp_base(components_bundle& components);

    public:
        virtual void render() = 0;

        void handle_on_start();
        void handle_on_finish(bool success);
        void handle_on_update(float progress);
        void handle_on_error(const std::string& msg);

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