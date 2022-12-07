#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cinttypes>

#include "B3L/BinaryPatch.h"
#include "Configuration.h"
#include "DInputHook.h"
#include "XInputHook.h"
#include <Windows.h>

class DS3 : public IXInputMap, public IDInputMap {
public:
    explicit DS3(const Configuration& config);

    void map(_XINPUT_STATE* pState) const override;
    void map(_DIMOUSESTATE2* state) const override;
    void map(char* keyboardState) const override;

private:
    B3L::BinaryPatch forceFxaaOnPatch;
};
