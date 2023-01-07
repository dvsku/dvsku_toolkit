#include "gui.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    dvsku::toolkit::gui::instance().start(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    return 0;
}
