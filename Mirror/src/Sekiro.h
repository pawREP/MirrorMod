#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cinttypes>

namespace Sekrio {

    extern intptr_t XInputGetStateOffset;

    void patch();

    void transformInput(XINPUT_STATE* state);

} // namespace Sekrio
