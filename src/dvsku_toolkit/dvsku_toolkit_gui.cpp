#include "app.hpp"

using namespace dvsku_toolkit;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    app app(600, 400);
    app.run();

    return 0;
}
