#include "Sekiro.h"
#include "DInputUtil.h"
#include "InlinePatchDeserializer.h"
#include "XInputUtil.h"

Sekiro::Sekiro(const B3L::ImageView& image, const Configuration& config) {
    const auto uiPatchDesc = config["PatchUI"].get<PatchDescriptor>();

    uiPatch = fromPatchDesc<B3L::InlinePatch>(image, uiPatchDesc);
    uiPatch->enable();
}

void Sekiro::map(_XINPUT_STATE* pState) const {
    XInputUtil::mirrorLeftStickX(pState);
}

void Sekiro::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void Sekiro::map(char* keyboardState) const {
    DInputUtil::mirrorWasdX(keyboardState);
}
