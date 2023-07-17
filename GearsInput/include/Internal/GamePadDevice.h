#pragma once
#include <Windows.h>
#include <Internal/DinputGamePad.h>
#include <Internal/XinputGamePad.h>
#include <array>
#include <variant>

namespace Glib::Internal::Input
{
    class GamePadDevice
    {
    public:
        using GamePad = std::variant<XinputGamePad, DinputGamePad>;

        enum class PadNumber : unsigned char
        {
            PAD_1 = 0,
            PAD_2 = 1,
            PAD_3 = 2,
            PAD_4 = 3
        };

        enum class InputType : unsigned short
        {
            Xinput = 0,
            Dinput = 1
        };

    public:
        bool Initialize(ComPtr<IDirectInput8> dinput);
        void Update();

    private:
        std::array<GamePad, 4> devices_;
    };
}
