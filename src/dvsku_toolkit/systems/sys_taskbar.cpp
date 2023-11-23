#include "systems/sys_taskbar.hpp"
#include "systems/systems_bundle.hpp"

#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

#include <versionhelpers.h>

using namespace dvsku_toolkit;

sys_taskbar::sys_taskbar(systems_bundle& systems) 
    : m_systems(systems) {}

void sys_taskbar::prepare() {
    if (!IsWindows7OrGreater()) return;

    HRESULT result = CoInitialize(NULL);

    if ((result != S_OK) && (result != S_FALSE)) return;

    ITaskbarList3* temp = NULL;
    result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&temp));

    if (!SUCCEEDED(result)) return;

    result = temp->HrInit();

    if (!SUCCEEDED(result)) return;

    m_taskbar = temp;
}

void sys_taskbar::release() {
    if (!m_taskbar) return;

    m_taskbar->Release();
    m_taskbar = nullptr;
}

void sys_taskbar::set_status(TBPFLAG flags) {
    if (!(*m_systems.window) || !m_taskbar)
        return;

    HWND handle = glfwGetWin32Window(*m_systems.window);
    if (!handle) 
        return;

    m_taskbar->SetProgressState(handle, flags);
}

void sys_taskbar::set_progress(float value) {
    if (!(*m_systems.window) || !m_taskbar)
        return;

    HWND handle = glfwGetWin32Window(*m_systems.window);
    if (!handle) return;

    m_taskbar->SetProgressValue(handle, (ULONGLONG)value, 100);
}
