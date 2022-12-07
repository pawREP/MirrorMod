#pragma once

#define WIN_LEAN_AND_MEAN
#include "B3L/Define.h"
#include <Windows.h>
#include <unknwn.h>

#ifdef DLL_EXPORT
#define DINPUT8_API __declspec(dllexport)
#else
#define DINPUT8_API __declspec(dllimport)
#endif

class DInput8Bridge {
    B3L_MAKE_NONCOPYABLE(DInput8Bridge);
    B3L_MAKE_NONMOVABLE(DInput8Bridge);

public:
    DInput8Bridge();
    ~DInput8Bridge();

    static HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

private:
    using DirectInput8Create_t = HRESULT (*)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

    HMODULE dinput8Module{};
    static inline DirectInput8Create_t originalDirectInput8Create = nullptr;
};

extern "C" {
DINPUT8_API HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
}
