#include "B3L/Debug.h"
#include "B3L/Hook.h"
#include "B3L/ScopeExit.h"
#include "DInput8.h"
#include <Windows.h>
#include <cassert>
#include <format>
#include <memory>

namespace {

    using GetCommandLineW_t     = LPWSTR (*)();
    using GetCommandLineWHook_t = B3L::IatHook<GetCommandLineW_t>;

    std::unique_ptr<DInput8Bridge> bridge                   = nullptr;
    std::unique_ptr<GetCommandLineWHook_t> initCallbackHook = nullptr;

    HMODULE mirrorModModule{};

    LPWSTR initCallback() {
        B3L::ScopeExit _([] { initCallbackHook.reset(nullptr); }); // Remove hook after initial call

        auto mirrorInit = reinterpret_cast<void (*)(void)>(GetProcAddress(mirrorModModule, "init"));
        assert(mirrorInit);
        mirrorInit();

        return GetCommandLineW();
    }

} // namespace

BOOL APIENTRY DllMain(HMODULE Module, DWORD ReasonForCall, LPVOID Reserved) {
    UNREFERENCED_PARAMETER(Module);
    UNREFERENCED_PARAMETER(Reserved);

    switch(ReasonForCall) {
    case DLL_PROCESS_ATTACH:
        // Setup original DInput8.dll
        bridge = std::make_unique<DInput8Bridge>();
        assert(bridge);

        // Load Mod
        mirrorModModule = LoadLibraryA("MirrorMod.dll");
        assert(mirrorModModule);

        // GetCommandLineW is hooked since this API is called early into main and allows us to initialize early on and 
        // safely without gambling with the limitations of DllMain that come as a result of the loader lock.
        try {
            initCallbackHook = std::make_unique<GetCommandLineWHook_t>("KERNEL32.dll", "GetCommandLineW", &initCallback);
            initCallbackHook->enable();
        } catch(...) {
            MessageBoxA(HWND{},
                        "Failed to set 'GetCommandLineW' based initialization hook. Application might "
                        "not be compatible with current hooking setup.",
                        "Error", UINT{});
        }

        break;
    }
    return TRUE;
}
