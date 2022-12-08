#include "DS2Sotfs.h"
#include "B3L/DeepPointer.h"
#include "B3L/Process.h"
#include "DInputUtil.h"
#include "DeepPointerDeserializer.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <vector>

DS2Sotfs::DS2Sotfs(const B3L::ImageView& image, const Configuration& config) {
    const auto isMenuOpenDesc = config["IsMenuOpen"].get<DeepPointerDescriptor>();
    const auto AAEnabledDesc  = config["AAEnabled"].get<DeepPointerDescriptor>();

    isMenuOpen = fromPointerDesc<bool>(image, isMenuOpenDesc);
    AAEnabled  = fromPointerDesc<int>(image, AAEnabledDesc);

    // TODO: Port the pathc below
    //   void* settingFixAddr = (void*)((long long)GetModuleHandle(NULL) + 0xEF6656);
    //  DWORD old;
    //  VirtualProtect(settingFixAddr, 9, PAGE_EXECUTE_READWRITE, &old);
    //  std::vector<unsigned char> patch = { 0xC6, 0x41, 0x20, 0x01, 0xC6, 0x42, 0x20, 0x01, 0x90 };
    //  memcpy(settingFixAddr, patch.data(), patch.size());
    //  VirtualProtect(settingFixAddr, 9, old, nullptr);
}

void DS2Sotfs::map(_XINPUT_STATE* pState) const {
    XInputUtil::mirrorLeftStickX(pState);

    if(isMenuOpen.getOr(false))
        return;

    XInputUtil::mirrorTiggers(pState);
    XInputUtil::mirrorBumpers(pState);
}

void DS2Sotfs::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void DS2Sotfs::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
