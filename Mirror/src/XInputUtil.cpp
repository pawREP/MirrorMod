#include "XInputUtil.h"
#include "B3L/Numeric.h"
#include <Windows.h>
#include <Xinput.h>
#include <utility>

void XInputUtil::mirrorLeftStickX(XINPUT_STATE* pState) {
    pState->Gamepad.sThumbLX = B3L::Numeric::negate(pState->Gamepad.sThumbLX);
    pState->Gamepad.sThumbRX = B3L::Numeric::negate(pState->Gamepad.sThumbRX);
}

void XInputUtil::mirrorTiggers(XINPUT_STATE* pState) {
    std::swap(pState->Gamepad.bLeftTrigger, pState->Gamepad.bRightTrigger);
}

void XInputUtil::mirrorBumpers(XINPUT_STATE* pState) {
    bool lbDown = pState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    bool rbDown = pState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

    if(lbDown)
        pState->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
    else
        pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;

    if(rbDown)
        pState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
    else
        pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;
}
