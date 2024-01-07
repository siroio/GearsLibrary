#pragma once
#include <Internal/DinputGamePad.h>
#include <Internal/XinputGamePad.h>
#include <Internal/GamePadKeys.h>
#include <ComPtr.h>
#include <array>
#include <variant>

namespace Glib::Internal::Input
{
    enum class PadNum : unsigned int
    {
        PAD_1 = 0,
        PAD_2 = 1,
        PAD_3 = 2,
        PAD_4 = 3,
        CAPACITY = 4,
    };

    class GamePadDevice
    {
    public:
        using GamePad = std::variant<XinputGamePad, DinputGamePad>;

    public:
        bool Initialize(ComPtr<IDirectInput8>& dinput);
        void Update();

        bool GetButton(PadNum pad, GPADKey button);
        bool GetButtonDown(PadNum pad, GPADKey button);
        bool GetButtonUp(PadNum pad, GPADKey button);
        Vector2 GetLeftStick(PadNum pad);
        Vector2 GetRightStick(PadNum pad);
        Vector2 GetLeftTrigger(PadNum pad);
        Vector2 GetRightTrigger(PadNum pad);
        void VibrateGamepad(PadNum padNum, float left, float right, float time);

    private:
        std::array<GamePad, 4> devices_;
    };
}
