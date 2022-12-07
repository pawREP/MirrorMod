#include "DInput8.h"
#include <cassert>

DINPUT8_API HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
    return DInput8Bridge::DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

DInput8Bridge::DInput8Bridge() {
    char dinputDllPath[MAX_PATH];
    GetSystemDirectoryA(dinputDllPath, MAX_PATH);
    strcat_s(dinputDllPath, "\\dinput8.dll");

    dinput8Module = LoadLibraryA(dinputDllPath);
    assert(dinput8Module);

    originalDirectInput8Create =
    reinterpret_cast<DirectInput8Create_t>(GetProcAddress(dinput8Module, "DirectInput8Create"));
    assert(originalDirectInput8Create);
}

DInput8Bridge::~DInput8Bridge() {
    FreeLibrary(dinput8Module);
}

HRESULT DInput8Bridge::DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
    assert(originalDirectInput8Create);

    return originalDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}
