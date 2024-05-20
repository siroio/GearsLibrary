#pragma once
#include <Internal/DirectInput8.h>
#include <Internal/IGamePad.h>
#include <ComPtr.h>
#include <array>

struct Vector2;

namespace Glib::Internal::Input
{
    class DinputGamePad : Interface::IGamePad
    {
    public:
        bool Initialize(ComPtr<IDirectInput8>& dinput);
        void Update();

        bool GetButton(GPADKey button) override;
        bool GetButtonDown(GPADKey button) override;
        bool GetButtonUp(GPADKey button) override;
        Vector2 GetLeftStick(float deadZone) override;
        Vector2 GetRightStick(float deadZone) override;
        float GetLeftTrigger(float deadZone) override;
        float GetRightTrigger(float deadZone) override;

    private:
        Vector2 GetStickPosition(long xAxis, long yAxis, float deadZone);
        float GetTriggerValue(long triggerValue, float deadZone);

    private:
        ComPtr<IDirectInputDevice8> device_{ nullptr };
        DIJOYSTATE currentState_;
        DIJOYSTATE prevState_;
    };
}
