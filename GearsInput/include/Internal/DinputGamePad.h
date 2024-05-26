#pragma once
#include <Internal/DirectInput8.h>
#include <Internal/IGamePad.h>
#include <ComPtr.h>
#include <array>

struct Vector2;

namespace Glib::Internal::Input
{
    class DinputGamePad
    {
    public:
        bool Initialize(ComPtr<IDirectInput8>& dinput);
        void Update();

        bool GetButton(GPADKey button);
        bool GetButtonDown(GPADKey button);
        bool GetButtonUp(GPADKey button);
        Vector2 GetLeftStick(float deadZone);
        Vector2 GetRightStick(float deadZone);
        float GetLeftTrigger(float deadZone);
        float GetRightTrigger(float deadZone);

    private:
        Vector2 GetStickPosition(long xAxis, long yAxis, float deadZone);
        float GetTriggerValue(long triggerValue, float deadZone);

    private:
        ComPtr<IDirectInputDevice8> device_{ nullptr };
        DIJOYSTATE currentState_;
        DIJOYSTATE prevState_;
    };
}
