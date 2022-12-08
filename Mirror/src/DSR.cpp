#include "DSR.h"
#include "DInputUtil.h"
#include "XInputUtil.h"

void DSR::map(_XINPUT_STATE* pState) const {
    XInputUtil::mirrorLeftStickX(pState);
}

void DSR::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void DSR::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
