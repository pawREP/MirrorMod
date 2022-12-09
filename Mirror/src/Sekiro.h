#pragma once
#include "B3L/PrologHook.h"
#include "Configuration.h"
#include "DInputHook.h"
#include "XInputHook.h"
#include <Windows.h>





class Sekiro : public IXInputMap, public IDInputMap {
public:
    Sekiro([[maybe_unused]] const B3L::ImageView& image, [[maybe_unused]] const Configuration& config) {
    };

    void map(_XINPUT_STATE* pState) const override;
    void map(_DIMOUSESTATE2* state) const override;
    void map(char* keyboardState) const override;
};
