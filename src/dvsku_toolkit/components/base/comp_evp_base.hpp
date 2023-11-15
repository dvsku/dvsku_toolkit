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
        void handle_on_finish(libevp::evp_result result);
        void handle_on_update(float progress);
        void handle_on_error(libevp::evp_result result);

    protected:
        components_bundle& m_components;

        bool m_cancel = false;

        std::string m_input  = "";
        std::string m_output = "";
        std::string m_key    = "";

        float m_progress = 0.0f;

    };
}