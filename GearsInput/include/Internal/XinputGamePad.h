#pragma once
#include <Internal/XinputGamePad.h>
#include <Internal/InputState.h>

namespace Glib::Internal::Input
{
    class XinputGamePad
    {
    public:
        bool Initialize(DWORD controllerIndex);
        void Update();

    private:
        DWORD controllerIndex_;
        InputState currentPadState_;
        InputState prevPadState_;
    };
}
