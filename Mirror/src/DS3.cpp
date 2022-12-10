#include "DS3.h"
#include "B3L/AOBScanner.h"
#include "DInputUtil.h"
#include "DeepPointerDeserializer.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <vector>

DS3::DS3(const B3L::ImageView& image, const Configuration& config) {
    const auto desc = config["FXAASetting"].get<DeepPointerDescriptor>();
    fxaaSetting     = fromPointerDesc<int>(image, desc);

    // Modify float used in routine that positions moveable ui elements. This fixes the screen space coordinates of lock-on dot and enemyHP bars on the mirrored render target.
    const auto needle  = B3L::AOBPattern::fromString("00 00 00 3F 00 00 00 BF 00 00 00 3F 00 00 00 00").value();
    const auto section = image.sectionData(2); //.rdata
    auto it            = B3L::AOBScanner::find(section.begin(), section.end(), needle);
    if(it != section.end())
        reinterpret_cast<float&>(const_cast<uint8_t&>(*it)) = -0.5;
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
