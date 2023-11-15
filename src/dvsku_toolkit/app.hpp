#pragma once

#include <windows.h>

#define GLFW_EXPOSE_NATIVE_WIN32

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

    private:
        void render();
    };
}
