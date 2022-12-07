#pragma once
#include "B3L/Hook.h"
#include "B3L/Unique.h"
#include <Windows.h>
#include <memory>
#include <mutex>

struct _XINPUT_STATE;

class IXInputMap {
public:
    virtual ~IXInputMap() = default;

    virtual void map(_XINPUT_STATE* pState) const;
};

class XInput {
public:
    enum class Version {
        XInput_Unkown,
        XInput_1_3,
        XInput_1_4,
    };

    XInput();
    ~XInput();

    void setInputMap(IXInputMap* map);

    static Version version();

private:
    using GetState_t     = DWORD(__stdcall*)(DWORD dwUserIndex, _XINPUT_STATE* pState);
    using GetStateHook_t = B3L::IatHook<GetState_t>;

    static DWORD GetState(DWORD dwUserIndex, _XINPUT_STATE* pState);

    static inline std::unique_ptr<GetStateHook_t> GetStateHook = nullptr;
    static inline std::mutex mut;

    // Input mapping for XInput state
    static inline IXInputMap* inputMap = nullptr;

    static inline const char* XINPUT1_3_Module               = "XINPUT1_3.dll";
    static inline const char* XINPUT1_4_Module               = "XINPUT1_4.dll";
    static inline const int XINPUT1_3_XInputGetState_Ordinal = 2;
    static inline const int XINPUT1_4_XInputGetState_Ordinal = 2;
};
