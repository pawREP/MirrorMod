#pragma once
#include "Configuration.h"
#include "DInputHook.h"
#include "XInputHook.h"
#include "B3L/DeepPointer.h"
#include <Windows.h>
#include <Xinput.h>
#include <cinttypes>

class DS2Sotfs : public IXInputMap, public IDInputMap {
public:
    DS2Sotfs(const B3L::ImageView& image, const Configuration& config);

    void map(_XINPUT_STATE* pState) const override;
    void map(_DIMOUSESTATE2* state) const override;
    void map(char* keyboardState) const override;

private:
    B3L::DeepPointer<bool> isMenuOpen;
    mutable B3L::DeepPointer<int> AAEnabled;
};
