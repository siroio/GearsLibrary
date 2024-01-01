#pragma once
#include <Internal/InputState.h>

namespace Glib::Internal::Input
{
    class XinputGamePad
    {
    public:
        bool Initialize(DWORD controllerIndex);
        void Update();

    private:
        DWORD controllerIndex_{ 0 };
        InputState currentPadState_{};
        InputState prevPadState_{};
    };
}

