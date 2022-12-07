#include "B3L/Debug.h"
#include "B3L/Hook.h"
#include "B3L/ScopeExit.h"
#include "DInput8.h"
#include <Windows.h>
#include <cassert>
#include <format>
#include <memory>

namespace {

    using GetSystemTimeAsFileTime_t     = void (*)(LPFILETIME lpSystemTimeAsFileTime);
    using GetSystemTimeAsFileTimeHook_t = B3L::IatHook<GetSystemTimeAsFileTime_t>;

    std::unique_ptr<DInput8Bridge> bridge                           = nullptr;
    std::unique_ptr<GetSystemTimeAsFileTimeHook_t> initCallbackHook = nullptr;

    HMODULE mirrorModModule{};

    // This function is called by the host process after loading the dll but before WinMain is entered in the host. Since DllMain
    // is serialized by the operating system there are major restrictions on what can be safely done in DllMain. Non-trivial initialization work should therefor be deferred to this function.
    void initCallback(LPFILETIME lpSystemTimeAsFileTime) {
        B3L::ScopeExit _([] { initCallbackHook.reset(nullptr); }); // Remove hook after initial call

        auto mirrorInit = reinterpret_cast<void (*)(void)>(GetProcAddress(mirrorModModule, "init"));
        assert(mirrorInit);
        mirrorInit();

        return GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
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

        // GetSystemTimeAsFileTime is hooked since this API is called as part of __secure_init_cookie before main is entered.
        // This callback allows initialization of loaded dlls without gambling with the limitations of DllMain that come as a result of the loader lock.
        try {
            initCallbackHook =
            std::make_unique<GetSystemTimeAsFileTimeHook_t>("KERNEL32.dll", "GetSystemTimeAsFileTime", &initCallback);
            initCallbackHook->enable();
        } catch(...) {
            MessageBoxA(HWND{},
                        "Failed to set 'GetSystemTimeAsFileTime' based initialization hook. Application might "
                        "not be compatible with current hooking setup.",
                        "Error", UINT{});
        }

        break;
    }
    return TRUE;
}
