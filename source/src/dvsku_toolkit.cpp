#include "dvsku_toolkit.hpp"
#include "dt_app.hpp"

#include <windows.h>

using namespace dvsku_toolkit;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    try {
        libutil::log::settings log_settings;
        log_settings.level = libutil::log::level::none;
        
        libutil::log::init(log_settings);

        libgui::window_settings settings;
        settings.width                 = 600U;
        settings.height                = 700U;
        settings.title                 = DT_APP_NAME;
        settings.enable_multi_viewport = true;

        dt_app app(settings);
        app.show();
    }
    catch (...) {
        return -1;
    }

    return 0;
}
