#include "Sekiro.h"

intptr_t XInputGetStateOffset = 0x2929BF8;

void Sekrio::patch() {
}

void Sekrio::transformInput(XINPUT_STATE* state) {
    UNREFERENCED_PARAMETER(state);
}
