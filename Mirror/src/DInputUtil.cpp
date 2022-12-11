#include "DInputUtil.h"
#include "B3L/Numeric.h"
#include <algorithm>
#include <dinput.h>

void DInputUtil::mirrorWasdX(char* keyboardState) {
    // TODO: Doesn't take any keymapping or anything into account. Should be fixed if possible
    std::swap(keyboardState[DIK_A], keyboardState[DIK_D]);
}

void DInputUtil::mirrorMouseX(_DIMOUSESTATE2* state) {
    state->lX = B3L::Numeric::negate(state->lX);
}
