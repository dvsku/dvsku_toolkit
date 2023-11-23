#pragma once

#include <windows.h>

#include "imgui.h"
#include "glfw/glfw3.h"

#include "components/components_bundle.hpp"
#include "systems/systems_bundle.hpp"

namespace dvsku_toolkit {
    class app {
    public:
        app(uint32_t width, uint32_t height);
        ~app();

        void run();

    private:
        GLFWwindow* m_window = nullptr;

        systems_bundle m_systems;
        components_bundle m_components;

        bool m_hovered_minimize = false;
        bool m_hovered_close    = false;
        bool m_active_minimize  = false;
        bool m_active_close     = false;

        bool m_can_move = false;

    private:
        void render();
        void on_window_move();
    };
}
