#pragma once

#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>

namespace dvsku_toolkit {
    class systems_bundle;

    class sys_taskbar {
    public:
        sys_taskbar() = delete;
        sys_taskbar(systems_bundle& systems);

        void prepare();
        void release();

        void set_status(TBPFLAG flags);
        void set_progress(float value);

    private:
        systems_bundle& m_systems;

        ITaskbarList3* m_taskbar = nullptr;
    };
}
