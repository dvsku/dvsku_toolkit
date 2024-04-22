#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace dvsku_toolkit {
    class dt_app;

    class dt_gui_base {
    public:
        dt_gui_base() = delete;
        dt_gui_base(dt_app& app) : m_app(app) {}

    protected:
        dt_app& m_app;
    };
}