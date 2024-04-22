#include "dvsku_toolkit.hpp"
#include "dt_app.hpp"

#include <windows.h>

using namespace dvsku_toolkit;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    try {
        dt_app app(600, 275, DT_APP_NAME);
        app.run();
    }
    catch (...) {
        return -1;
    }

    return 0;
}