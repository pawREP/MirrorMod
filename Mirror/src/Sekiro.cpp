#include "Sekiro.h"
#include "DInputUtil.h"
#include "XInputUtil.h"

void Sekiro::map(_XINPUT_STATE* pState) const {
    XInputUtil::mirrorLeftStickX(pState);
}

void Sekiro::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void Sekiro::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
