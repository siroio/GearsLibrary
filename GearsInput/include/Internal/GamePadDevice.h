#pragma once
#include <Internal/DinputGamePad.h>
#include <Internal/XinputGamePad.h>
#include <Internal/GamePadKeys.h>
#include <array>
#include <variant>

namespace Glib::Internal::Input
{
    class GamePadDevice
    {
    public:
        using GamePad = std::variant<XinputGamePad, DinputGamePad>;

        enum class PadNum : unsigned int
        {
            PAD_1 = 0,
            PAD_2 = 1,
            PAD_3 = 2,
            PAD_4 = 3
        };

        enum class InputType : unsigned int
        {
            Xinput = 0,
            Dinput = 1
        };

    public:
        bool Initialize(ComPtr<IDirectInput8>& dinput);
        void Update();

        //bool GetButton(PadNum pad, GPADCode button);
        //bool GetButtonDown(PadNum pad, GPADCode button);
        //bool GetButtonUp(PadNum pad, GPADCode button);
        //bool GetAxis(PadNum pad, unsigned short axis);

    private:
        std::array<GamePad, 4> devices_;
    };
}
