#include "DSR.h"
#include "DInputUtil.h"
#include "XInputUtil.h"
#include <format>

namespace {

    // Original GetAsyncKeyState function
    SHORT (*getAsyncKeyState)(int) = nullptr;

    // Swap state for A and D key codes
    SHORT remapKeystate(int vKey) {
        if(vKey == 'A')
            vKey = 'D';
        else if(vKey == 'D')
            vKey = 'A';

        return getAsyncKeyState(vKey);
    }

} // namespace

DSR::DSR([[maybe_unused]] const B3L::ImageView& image, [[maybe_unused]] const Configuration& config) {
    // Unlike all other recent From Software games DSR doesn't utilize DINPUT to query the current keyboard state.
    // GetAsyncKeyState is used instead which means we need an additional input hook here.

    getAsyncKeyStateHook = std::make_unique<B3L::IatHook<SHORT (*)(int)>>("USER32.dll", "GetAsyncKeyState", &remapKeystate);
    getAsyncKeyState = getAsyncKeyStateHook->originalTarget<SHORT(int)>();

    getAsyncKeyStateHook->enable();
}

void DSR::map(_XINPUT_STATE* pState) const {
    XInputUtil::mirrorLeftStickX(pState);
}

void DSR::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void DSR::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
