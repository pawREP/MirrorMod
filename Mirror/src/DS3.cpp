#include "DS3.h"
#include "B3L/AOBScanner.h"
#include "DInputUtil.h"
#include "InlinePatchDeserializer.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <vector>

// namespace DS3 {
//
//     intptr_t XInputGetStateOffset = 0x4DBC05C;
//
//     void patch() {
//         // Permanently sets FXAA to one. This is required since the FXAA shader is used for mirroring
//         void* settingFixAddr = (void*)((intptr_t)GetModuleHandle(NULL) + 0xEF6656);
//         DWORD old;
//         std::vector<unsigned char> patch = { 0xC6, 0x41, 0x20, 0x01, 0xC6, 0x42, 0x20, 0x01, 0x90 };
//         VirtualProtect(settingFixAddr, patch.size(), PAGE_EXECUTE_READWRITE, &old);
//         memcpy(settingFixAddr, patch.data(), patch.size());
//         VirtualProtect(settingFixAddr, patch.size(), old, &old);
//     }
//
//     void transformInput(XINPUT_STATE* state) {
//         XInputUtil::mirrorLeftStickX(state);
//     }
//
// } // namespace DS3

DS3::DS3(const Configuration& config) {
    // TODO: fix and reenable

    // auto imageBase       = B3L::getModuleBaseAddress();
    // const auto imageView = B3L::ImageView::createFromMappedImage(imageBase);
    // if(!imageView)
    //     throw std::runtime_error("ImageView construction failed");

    // auto desc        = config["ForceFXAAOn"].get<InlinePatchDescriptor>();
    // forceFxaaOnPatch = fromPatchDesc<B3L::BinaryPatch>(imageView.value(), desc);
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
