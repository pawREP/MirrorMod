#pragma once

struct _XINPUT_STATE;

namespace XInputUtil {

    void mirrorLeftStickX(_XINPUT_STATE* pState);
    void mirrorTiggers(_XINPUT_STATE* pState);
    void mirrorBumpers(_XINPUT_STATE* pState);

}
