#include "DInputHook.h"
#include "B3L/Numeric.h"
#include "B3L/Unreachable.h"
#include "Game.h"
#include "VTable.h"
#include <Unknwn.h>
#include <Windows.h>
#include <dinput.h>

#pragma comment(lib, "dxguid")


template <>
struct VTable<IDirectInputDevice> {
    HRESULT (*QueryInterface)(IDirectInputDevice* this_, REFIID riid, LPVOID* ppvObj);
    ULONG (*AddRef)(IDirectInputDevice* this_);
    ULONG (*Release)(IDirectInputDevice* this_);
    HRESULT (*GetCapabilities)(IDirectInputDevice* this_, LPDIDEVCAPS);
    HRESULT (*EnumObjects)(IDirectInputDevice* this_, LPDIENUMDEVICEOBJECTSCALLBACKW, LPVOID, DWORD);
    HRESULT (*GetProperty)(IDirectInputDevice* this_, REFGUID, LPDIPROPHEADER);
    HRESULT (*SetProperty)(IDirectInputDevice* this_, REFGUID, LPCDIPROPHEADER);
    HRESULT (*Acquire)(IDirectInputDevice* this_);
    HRESULT (*Unacquire)(IDirectInputDevice* this_);
    HRESULT (*GetDeviceState)(IDirectInputDevice* this_, DWORD, LPVOID);
    HRESULT (*GetDeviceData)(IDirectInputDevice* this_, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);
    HRESULT (*SetDataFormat)(IDirectInputDevice* this_, LPCDIDATAFORMAT);
    HRESULT (*SetEventNotification)(IDirectInputDevice* this_, HANDLE);
    HRESULT (*SetCooperativeLevel)(IDirectInputDevice* this_, HWND, DWORD);
    HRESULT (*GetObjectInfo)(IDirectInputDevice* this_, LPDIDEVICEOBJECTINSTANCEW, DWORD, DWORD);
    HRESULT (*GetDeviceInfo)(IDirectInputDevice* this_, LPDIDEVICEINSTANCEW);
    HRESULT (*RunControlPanel)(IDirectInputDevice* this_, HWND, DWORD);
    HRESULT (*Initialize)(IDirectInputDevice* this_, HINSTANCE, DWORD, REFGUID);
    HRESULT (*CreateEffect)(IDirectInputDevice* this_, REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT*, LPUNKNOWN);
    HRESULT (*EnumEffects)(IDirectInputDevice* this_, LPDIENUMEFFECTSCALLBACKW, LPVOID, DWORD);
    HRESULT (*GetEffectInfo)(IDirectInputDevice* this_, LPDIEFFECTINFOW, REFGUID);
    HRESULT (*GetForceFeedbackState)(IDirectInputDevice* this_, LPDWORD);
    HRESULT (*SendForceFeedbackCommand)(IDirectInputDevice* this_, DWORD);
    HRESULT(*EnumCreatedEffectObjects)
    (IDirectInputDevice* this_, LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD);
    HRESULT (*Escape)(IDirectInputDevice* this_, LPDIEFFESCAPE);
    HRESULT (*Poll)(IDirectInputDevice* this_);
    HRESULT (*SendDeviceData)(IDirectInputDevice* this_, DWORD, LPCDIDEVICEOBJECTDATA, LPDWORD, DWORD);
    HRESULT (*EnumEffectsInFile)(IDirectInputDevice* this_, LPCWSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD);
    HRESULT (*WriteEffectToFile)(IDirectInputDevice* this_, LPCWSTR, DWORD, LPDIFILEEFFECT, DWORD);
    HRESULT (*BuildActionMap)(IDirectInputDevice* this_, LPDIACTIONFORMATW, LPCWSTR, DWORD);
    HRESULT (*SetActionMap)(IDirectInputDevice* this_, LPDIACTIONFORMATW, LPCWSTR, DWORD);
    HRESULT (*GetImageInfo)(IDirectInputDevice* this_, LPDIDEVICEIMAGEINFOHEADERW);
};

template <>
struct VTable<IDirectInput> {
    HRESULT (*QueryInterface)(IDirectInput* this_, REFIID riid, LPVOID* ppvObj);
    ULONG (*AddRef)(IDirectInput* this_);
    ULONG (*Release)(IDirectInput* this_);
    HRESULT (*CreateDevice)(IDirectInput*, REFGUID, LPDIRECTINPUTDEVICE*, LPUNKNOWN) PURE;
    HRESULT (*EnumDevices)(IDirectInput*, DWORD, LPDIENUMDEVICESCALLBACKW, LPVOID, DWORD) PURE;
    HRESULT (*GetDeviceStatus)(IDirectInput*, REFGUID) PURE;
    HRESULT (*RunControlPanel)(IDirectInput*, HWND, DWORD) PURE;
    HRESULT (*Initialize)(IDirectInput*, HINSTANCE, DWORD) PURE;
    HRESULT (*FindDevice)(IDirectInput*, REFGUID, LPCWSTR, LPGUID) PURE;
    HRESULT(*EnumDevicesBySemantics)
    (IDirectInput*, LPCWSTR, LPDIACTIONFORMATW, LPDIENUMDEVICESBYSEMANTICSCBW, LPVOID, DWORD) PURE;
    HRESULT(*ConfigureDevices)
    (IDirectInput*, LPDICONFIGUREDEVICESCALLBACK, LPDICONFIGUREDEVICESPARAMSW, DWORD, LPVOID) PURE;
};

DInput::DInput() {
    std::lock_guard lock(mut);

    DirectInput8CreateHook =
    std::make_unique<DirectInput8CreateHook_t>("DINPUT8.dll", "DirectInput8Create", &DInput::DirectInput8Create);

    DirectInput8CreateHook->enable();
}

DInput::~DInput() {
    std::lock_guard lock(mut);

    inputMap = nullptr;

    DirectInput8CreateDeviceHook.reset();
    DirectInput8CreateHook.reset();
    GetDeviceStateHooks.clear();
}

void DInput::setInputMap(IDInputMap* map) {
    std::lock_guard lock(mut);

    inputMap = map;
}

HRESULT DInput::CreateDevice(IDirectInput* this_, REFGUID rguid, LPDIRECTINPUTDEVICE* lplpDirectInputDevice, LPUNKNOWN pUnkOuter) {
    std::lock_guard lock(mut);

    if(!DirectInput8CreateDeviceHook)
        return this_->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);

    auto hr = DirectInput8CreateDeviceHook->invokeOriginal(this_, rguid, lplpDirectInputDevice, pUnkOuter);

    // Different devices may share vtables, we have to ensure that we only hook once per table regardless of device. Hooking per unique GUID is not safe.
    auto vft = *(void**)(*lplpDirectInputDevice);
    if(!GetDeviceStateHooks.contains(vft)) {
        GetDeviceStateHooks[vft] =
        std::make_unique<DirectInputDevice8GetDeviceStateHook_t>(*lplpDirectInputDevice,
                                                                 &VTable<IDirectInputDevice>::GetDeviceState, &GetDeviceState);
        GetDeviceStateHooks[vft]->enable();
    }

    return hr;
}

HRESULT DInput::GetDeviceState(IDirectInputDevice* this_, DWORD cbData, LPVOID lpvData) {
    std::lock_guard lock(mut);

    auto vtf = *(void**)(this_);
    if(!GetDeviceStateHooks.contains(vtf))
        return this_->GetDeviceState(cbData, lpvData);

    auto hr = GetDeviceStateHooks[vtf]->invokeOriginal(this_, cbData, lpvData);

    switch(cbData) {
    case sizeof(DIMOUSESTATE2): {
        DIMOUSESTATE2* state = reinterpret_cast<DIMOUSESTATE2*>(lpvData);
        if(inputMap)
            inputMap->map(state);

        return hr;
    } break;

    case 256: {
        char* state = reinterpret_cast<char*>(lpvData);
        if(inputMap)
            inputMap->map(state);

        return hr;
    } break;

    default:
        unreachable();
    }
}

HRESULT DInput::DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
    std::lock_guard lock(mut);

    if(!DirectInput8CreateHook)
        return DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);

    auto hr = DirectInput8CreateHook->invokeOriginal(hinst, dwVersion, riidltf, ppvOut, punkOuter);

    if(!DirectInput8CreateDeviceHook) {
        auto instance = *reinterpret_cast<LPDIRECTINPUT*>(ppvOut);
        DirectInput8CreateDeviceHook =
        std::make_unique<DirectInput8CreateDeviceHook_t>(instance, &VTable<IDirectInput>::CreateDevice, &CreateDevice);

        DirectInput8CreateDeviceHook->enable();
    }

    return hr;
}

void IDInputMap::map(DIMOUSESTATE2* state) const {
    UNREFERENCED_PARAMETER(state);
}

void IDInputMap::map(char* keyboardState) const {
    UNREFERENCED_PARAMETER(keyboardState);
}
