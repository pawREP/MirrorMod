#include "DS3.h"
#include "B3L/AOBScanner.h"
#include "DInputUtil.h"
#include "InlinePatchDeserializer.h"
#include "DeepPointerDeserializer.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <vector>

DS3::DS3(const B3L::ImageView& image, const Configuration& config) {
    auto desc = config["FXAASetting"].get<DeepPointerDescriptor>();
    fxaaSetting = fromPointerDesc<int>(image, desc);
}

void DS3::map(_XINPUT_STATE* state) const {
    XInputUtil::mirrorLeftStickX(state);
}
void DS3::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void DS3::map(char* keyboardState) const {
    fxaaSetting.set(1);
    DInputUtil::mirrorWasdX(keyboardState);
}
