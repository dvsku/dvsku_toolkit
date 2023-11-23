#include "systems/sys_sound.hpp"

#include <windows.h>
#include <playsoundapi.h>

using namespace dvsku_toolkit;

void sys_sound::play_error_sound() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}

void sys_sound::play_success_sound() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}
