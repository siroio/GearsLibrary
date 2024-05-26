#pragma once
#include <Internal/DinputGamePad.h>
#include <Internal/XinputGamePad.h>
#include <Internal/IGamePad.h>
#include <Internal/GamePadKeys.h>
#include <array>
#include <memory>

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
        bool Initialize();
        void Update();

        bool GetButton(PadNum pad, GPADKey button);
        bool GetButtonDown(PadNum pad, GPADKey button);
        bool GetButtonUp(PadNum pad, GPADKey button);
        Vector2 GetLeftStick(PadNum pad, float deadZone);
        Vector2 GetRightStick(PadNum pad, float deadZone);
        float GetLeftTrigger(PadNum pad, float deadZone);
        float GetRightTrigger(PadNum pad, float deadZone);
        void VibrateGamepad(PadNum padNum, float left, float right, float time);

    private:
        std::array<std::unique_ptr<Interface::IGamePad>, 4> devices_;
    };
}
