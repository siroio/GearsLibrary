#pragma once
#include <Windows.h>
#include <Xinput.h>

namespace Glib::Internal::Input
{
    struct InputState
    {
        XINPUT_STATE state{};
        float VibratingStartTime{ 0.0f };
        float VibratingTime{ 0.0f };
        bool isConnected{ true };
        bool isVibrating{ false };
    };
}
