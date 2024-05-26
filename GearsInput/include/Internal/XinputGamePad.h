#pragma once
#include <Internal/XInputState.h>
#include <Internal/IGamePad.h>

namespace Glib::Internal::Input
{
    class XinputGamePad : public Interface::IGamePad
    {
    public:
        bool Initialize(DWORD controllerIndex) override;
        void Update() override;

        bool GetButton(GPADKey button) override;
        bool GetButtonDown(GPADKey button) override;
        bool GetButtonUp(GPADKey button) override;
        Vector2 GetLeftStick(float deadZone) override;
        Vector2 GetRightStick(float deadZone) override;
        float GetLeftTrigger(float deadZone) override;
        float GetRightTrigger(float deadZone) override;
        void VibratePad(float left, float right, float time) override;

    private:
        bool CheckConnection() const;

    private:
        DWORD controllerIndex_{ 0 };
        XInputState currentPadState_{};
        XInputState prevPadState_{};
    };
}
