#include "B3L/Debug.h"
#include "B3L/ImageView.h"
#include "B3L/Process.h"
#include "B3L/ScopeExit.h"
#include "Configuration.h"
#include "DInputHook.h"
#include "Game.h"
#include "XInputHook.h"
#include <Windows.h>
#include <cassert>
#include <format>
#include <memory>

namespace {

    std::unique_ptr<XInput> xinput = nullptr;
    std::unique_ptr<DInput> dinput = nullptr;
    std::unique_ptr<Game> game     = nullptr;

} // namespace

void initialize() {
    B3L::ScopeFailure cleanup([]() {
        if(xinput)
            xinput.reset();
        if(dinput)
            dinput.reset();
        if(game)
            game.reset();
    });

    try {
        xinput = std::make_unique<XInput>();
        dinput = std::make_unique<DInput>();
    } catch(const std::exception& e) {
        throw std::runtime_error(std::format("Hooking inputs failed with error: {}", e.what()));
    }

    const auto moduleBase = B3L::getModuleBaseAddress();
    const auto imageView  = B3L::ImageView::createFromMappedImage(moduleBase);
    if(!imageView)
        throw std::runtime_error("Parsing mapped image failed");

    Configuration configuration;
    try {
        auto exeName  = B3L::getExecutableFileName();
        configuration = Configuration{ exeName, imageView->timestamp() };
    } catch(const std::exception& e) {
        throw std::runtime_error(std::format("Parsing Configuration file failed with error: {}", e.what()));
    }

    try {
        game = std::make_unique<Game>(imageView.value(), configuration);
    } catch(const std::exception& e) {
        throw std::runtime_error(std::format("Creating mirror mod instance failed with error: {}", e.what()));
    }

    xinput->setInputMap(game.get());
    dinput->setInputMap(game.get());
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    switch(ul_reason_for_call) {
    case DLL_PROCESS_DETACH:
        xinput.reset();
        dinput.reset();
        game.reset();
        break;
    }
    return TRUE;
}

extern "C" {
__declspec(dllexport) void init() {
    try {
        initialize();
    } catch(const std::exception& e) {
        const auto errorMsg =
        std::format("MirrorMod encountered an error while loading!\n\nError:\n{}\n\nPress OK to exit the program.", e.what());
        MessageBoxA(HWND{}, errorMsg.c_str(), "Error", MB_OK | MB_ICONEXCLAMATION);
        std::exit(EXIT_FAILURE);
    }
}
}
