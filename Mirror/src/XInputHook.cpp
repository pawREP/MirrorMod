#include "XInputHook.h"
#include "Game.h"
#include <Windows.h>
#include <Xinput.h>
#include <algorithm>
#include <mutex>

XInput::XInput() {
    std::lock_guard lock(mut);

    switch(version()) {
    case Version::XInput_1_3:
        GetStateHook = std::make_unique<GetStateHook_t>(XINPUT1_3_Module, XINPUT1_3_XInputGetState_Ordinal, &XInput::GetState);
        break;
    case Version::XInput_1_4:
        GetStateHook = std::make_unique<GetStateHook_t>(XINPUT1_4_Module, XINPUT1_4_XInputGetState_Ordinal, &XInput::GetState);
        break;
    default:
        throw std::runtime_error("No supported XInput module found");
    }

    if(GetStateHook)
        GetStateHook->enable();
}

DWORD XInput::GetState(DWORD dwUserIndex, _XINPUT_STATE* pState) {
    std::lock_guard lock(mut);

    if(!GetStateHook)
        return 0;

    auto ret = GetStateHook->invokeOriginal(dwUserIndex, pState);

    if(inputMap)
        inputMap->map(pState);

    return ret;
}

XInput::~XInput() {
    std::lock_guard lock(mut);

    GetStateHook.reset();
    inputMap = nullptr;
}

void XInput::setInputMap(IXInputMap* map) {
    std::lock_guard lock(mut);

    inputMap = map;
}

XInput::Version XInput::version() {
    // TODO: Could multiple versions be loaded at the same time?
    if(GetModuleHandleA("XINPUT1_3"))
        return Version::XInput_1_3;
    if(GetModuleHandleA("XINPUT1_4"))
        return Version::XInput_1_4;

    return Version::XInput_Unkown;
}

void IXInputMap::map(_XINPUT_STATE* pState) const {
    UNREFERENCED_PARAMETER(pState);
}
