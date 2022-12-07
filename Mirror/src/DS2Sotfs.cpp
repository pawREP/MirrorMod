#include "DS2Sotfs.h"
#include "B3L/DeepPointer.h"
#include "B3L/Process.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <vector>

namespace DS2Sotfs {

    intptr_t XInputGetStateOffset = 0x1a636ac;
    // DeepPointer isMenuOpenDp{ (intptr_t)GetModuleHandle(NULL), std::initializer_list{ 0x160B8D0, 0x22E0, 0x10, 0x08 } };
    B3L::DeepPointer<bool> isMenuOpenDp{ B3L::getModuleBaseAddress(), { 0x160B8D0, 0x22E0, 0x10, 0x08 } };
    B3L::DeepPointer<int> AAEnabled{ B3L::getModuleBaseAddress(), { 0x166C1D8, 0x40, 0xCC8 } };

    void patch() {

        // void* settingFixAddr = (void*)((long long)GetModuleHandle(NULL) + 0xEF6656);
        // DWORD old;
        // VirtualProtect(settingFixAddr, 9, PAGE_EXECUTE_READWRITE, &old);
        // std::vector<unsigned char> patch = { 0xC6, 0x41, 0x20, 0x01, 0xC6, 0x42, 0x20, 0x01, 0x90 };
        // memcpy(settingFixAddr, patch.data(), patch.size());
        // VirtualProtect(settingFixAddr, 9, old, nullptr);
    }

    bool isMenuOpen() {
        AAEnabled.set(1);
        return isMenuOpenDp.getOr(true);
    }

    void transformInput(XINPUT_STATE* state) {
        XInputUtil::mirrorLeftStickX(state);

        if(isMenuOpen())
            return;

        XInputUtil::mirrorTiggers(state);
        XInputUtil::mirrorBumpers(state);
    }

} // namespace DS2Sotfs