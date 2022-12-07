#pragma once
#include <cinttypes>
#include <Windows.h>
#include <Xinput.h>

namespace DS2Sotfs {

    extern intptr_t XInputGetStateOffset;

    void patch();
    bool isMenuOpen();

    void transformInput(XINPUT_STATE* state);

} // namespace Game
