#include "DS3.h"
#include "B3L/AOBScanner.h"
#include "DInputUtil.h"
#include "InlinePatchDeserializer.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <vector>

}

void DS3::map(_XINPUT_STATE* state) const {
    XInputUtil::mirrorLeftStickX(state);
}
void DS3::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void DS3::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
