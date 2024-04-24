#include "dvsku_toolkit.hpp"
#include "dt_app.hpp"

#include <windows.h>

using namespace dvsku_toolkit;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    try {
        dvsku::dv_window_settings settings;
        settings.width                 = 600U;
        settings.height                = 275U;
        settings.title                 = DT_APP_NAME;
        settings.enable_multi_viewport = true;

        dt_app app(settings);
        app.run();
    }
    catch (...) {
        return -1;
    }

    return 0;
}