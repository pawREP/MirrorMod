#include "DSR.h"
#include "DInputUtil.h"
#include "XInputUtil.h"

// namespace DSR {
//
//     intptr_t XInputGetStateOffset = 0x20B5CA0;
//
//     void patch() {
//     }
//
//     void transformInput(XINPUT_STATE* state) {
//         XInputUtil::mirrorLeftStickX(state);
//     }
//
// } // namespace DSR

void DSR::map(_XINPUT_STATE* pState) const {
    XInputUtil::mirrorLeftStickX(pState);
}

void DSR::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void DSR::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
