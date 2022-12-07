#pragma once

struct _DIMOUSESTATE2;

namespace DInputUtil {

    void mirrorWasdX(char* keyboardState);
    void mirrorMouseX(_DIMOUSESTATE2* state);

} // namespace DInputUtil
