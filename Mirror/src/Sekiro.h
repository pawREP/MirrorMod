#pragma once
#include "B3L/InlineCallbackHook.h"
#include "Configuration.h"
#include "DInputHook.h"
#include "XInputHook.h"
#include <Windows.h>

class Sekiro : public IXInputMap, public IDInputMap {
public:
    Sekiro([[maybe_unused]] const B3L::ImageView& image, const Configuration& config);

    void map(_XINPUT_STATE* pState) const override;
    void map(_DIMOUSESTATE2* state) const override;
    void map(char* keyboardState) const override;

private:
    std::unique_ptr<B3L::InlinePatch> uiPatch = nullptr;
};
