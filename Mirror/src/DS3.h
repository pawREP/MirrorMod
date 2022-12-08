#pragma once
#include "B3L/InlinePatch.h"
#include "B3L/DeepPointer.h"
#include "Configuration.h"
#include "DInputHook.h"
#include "XInputHook.h"
#include <Windows.h>
#include <Xinput.h>
#include <cinttypes>

class DS3 : public IXInputMap, public IDInputMap {
public:
    DS3(const B3L::ImageView& image, const Configuration& config);

    void map(_XINPUT_STATE* pState) const override;
    void map(_DIMOUSESTATE2* state) const override;
    void map(char* keyboardState) const override;

private:
    mutable B3L::DeepPointer<int> fxaaSetting;
};
