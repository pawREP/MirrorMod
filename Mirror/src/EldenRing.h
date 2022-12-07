#pragma once
#include "B3L/DeepPointer.h"
#include "B3L/InlinePatch.h"
#include "Configuration.h"
#include "DInputHook.h"
#include "XInputHook.h"
#include <Windows.h>
#include <cinttypes>

class EldenRing : public IXInputMap, public IDInputMap {
public:
    enum MenuStateMask : uint32_t {
        InGameMenuOpen = 1 << 4,
        MapOpen        = 1 << 8,
        SortChestOpen  = 1 << 17,
    };

    EldenRing(const B3L::ImageView& image, const Configuration& config);

    void map(_XINPUT_STATE* pState) const override;
    void map(_DIMOUSESTATE2* state) const override;
    void map(char* keyboardState) const override;

private:
    bool queryMenuState(uint32_t mask) const;

    static bool isGameUnpacked();

    std::unique_ptr<B3L::InlinePatch> enemyHpPatch = nullptr;
    std::unique_ptr<B3L::InlinePatch> lockonPatch  = nullptr;
    B3L::DeepPointer<uint32_t> menuState;
    B3L::DeepPointer<bool> isInGame;
};
