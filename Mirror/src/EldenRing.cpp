#include "EldenRing.h"
#include "B3L/DeepPointer.h"
#include "DInputUtil.h"
#include "DeepPointerDeserializer.h"
#include "InlinePatchDeserializer.h"
#include "XInputUtil.h"
#include <Windows.h>
#include <cassert>
#include <cstddef>
#include <dinput.h>
#include <filesystem>
#include <format>

EldenRing::EldenRing(const B3L::ImageView& image, const Configuration& config) {
    if(!isGameUnpacked()) {
        const auto err =
        "Game not unpacked! Mirror mod requires the game archives to be unpacked and the executable "
        "be patched before the mod is installed. Unpacking can be done with 'UMX Selective Unpacker' which can "
        "found on Nexus Mods (https://www.nexusmods.com/eldenring/mods/1651). After unpacking the game, reinstall "
        "the mod and try again.";

        throw std::runtime_error(err);
    }

    const auto menuStatePtrDesc = config["MenuState"].get<DeepPointerDescriptor>();
    const auto isInGamePtrDesc  = config["IsInGame"].get<DeepPointerDescriptor>();
    const auto enemyHpPatchDesc = config["EnemyHPBarPatch"].get<PatchDescriptor>();
    const auto lockonPatchDesc  = config["LockonPatch"].get<PatchDescriptor>();

    menuState    = fromPointerDesc<uint32_t>(image, menuStatePtrDesc);
    isInGame     = fromPointerDesc<bool>(image, isInGamePtrDesc);
    enemyHpPatch = fromPatchDesc<B3L::InlinePatch>(image, enemyHpPatchDesc);
    lockonPatch  = fromPatchDesc<B3L::InlinePatch>(image, lockonPatchDesc);

    enemyHpPatch->enable();
    lockonPatch->enable();
}

void EldenRing::map(_XINPUT_STATE* pState) const {
    if(!isInGame.getOr(false))
        return;

    if(!queryMenuState(MapOpen))
        XInputUtil::mirrorLeftStickX(pState);
}

void EldenRing::map(_DIMOUSESTATE2* state) const {
    DInputUtil::mirrorMouseX(state);
}

void EldenRing::map(char* keyboardState) const {
    if(!isInGame.getOr(false))
        return;

    if(!queryMenuState(MapOpen))
        DInputUtil::mirrorWasdX(keyboardState);
}

bool EldenRing::queryMenuState(uint32_t mask) const {
    auto v = menuState.getOr(0u);
    return v & mask;
}

bool EldenRing::isGameUnpacked() {
    const auto shaderDir = std::filesystem::current_path() / "shader";
    return std::filesystem::exists(shaderDir) && std::filesystem::is_directory(shaderDir);
}
