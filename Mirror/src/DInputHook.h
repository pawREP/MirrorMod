#pragma once
#include "B3L/Hook.h"
#include "B3L/Unique.h"
#include <Windows.h>
#include <dinput.h>

class IDInputMap {
public:
    virtual ~IDInputMap() = default;

    virtual void map(DIMOUSESTATE2* state) const;
    virtual void map(char* keyboardState) const;
};

class DInput { 
public:
    DInput();
    ~DInput();

    void setInputMap(IDInputMap* map);

private:
    using DirectInput8Create_t = HRESULT (*)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
    using DirectInput8CreateDevice_t = HRESULT (*)(IDirectInput* this_, REFGUID rguid, LPDIRECTINPUTDEVICE* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    using DirectInputDevice8GetDeviceState_t = HRESULT (*)(IDirectInputDevice* this_, DWORD cbData, LPVOID lpvData);

    using DirectInput8CreateHook_t               = B3L::IatHook<DirectInput8Create_t>;
    using DirectInput8CreateDeviceHook_t         = B3L::VftHook<DirectInput8CreateDevice_t>;
    using DirectInputDevice8GetDeviceStateHook_t = B3L::VftHook<DirectInputDevice8GetDeviceState_t>;

    static HRESULT GetDeviceState(IDirectInputDevice* this_, DWORD cbData, LPVOID lpvData);
    static inline std::unique_ptr<DirectInputDevice8GetDeviceStateHook_t> GetKeyboardDeviceStateHook = nullptr;
    static inline std::unique_ptr<DirectInputDevice8GetDeviceStateHook_t> GetMouseDeviceStateHook    = nullptr;

    static HRESULT CreateDevice(IDirectInput* this_, REFGUID rguid, LPDIRECTINPUTDEVICE* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    static inline std::unique_ptr<DirectInput8CreateDeviceHook_t> DirectInput8CreateDeviceHook = nullptr;

    static HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
    static inline std::unique_ptr<DirectInput8CreateHook_t> DirectInput8CreateHook = nullptr;

    // Input mapping for XInput state
    static inline IDInputMap* inputMap = nullptr;

    static inline std::mutex mut;
};
