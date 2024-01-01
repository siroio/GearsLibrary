#pragma once
#include <Internal/XinputGamePad.h>
#include <GameTimer.h>

bool Glib::Internal::Input::XinputGamePad::Initialize(DWORD controllerIndex)
{
    return false;
}

void Glib::Internal::Input::XinputGamePad::Update()
{
    float now = GameTimer::Now();
    if (currentPadState_.isConnected) return;
    prevPadState_ = currentPadState_;
    if (XInputGetState(controllerIndex_, &currentPadState_.state) == ERROR_DEVICE_NOT_CONNECTED)
    {
        currentPadState_.isConnected = false;
        return;
    }

    if (currentPadState_.isVibrating) return;

    // 振動停止
    if (now - currentPadState_.vibratingStartTime >= currentPadState_.vibratingTime)
    {
        XINPUT_VIBRATION vibration{};
        vibration.wLeftMotorSpeed = 0;
        vibration.wRightMotorSpeed = 0;
        XInputSetState(controllerIndex_, &vibration);
        currentPadState_.isVibrating = false;
        currentPadState_.vibratingStartTime = 0.0f;
        currentPadState_.vibratingTime = 0.0f;
    }
}

