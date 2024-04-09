#pragma once
#include <Internal/XinputGamePad.h>
#include <GameTimer.h>
#include <Vector2.h>
#include <Debugger.h>

bool Glib::Internal::Input::XinputGamePad::Initialize(DWORD controllerIndex)
{
    controllerIndex_ = controllerIndex;
    currentPadState_.isConnected = XInputGetState(controllerIndex_, &currentPadState_.state) == ERROR_SUCCESS;
    return true;
}

void Glib::Internal::Input::XinputGamePad::Update()
{
    float now = GameTimer::Now();

    prevPadState_ = currentPadState_;
    auto res = XInputGetState(controllerIndex_, &currentPadState_.state);

    if (res != ERROR_SUCCESS)
    {
        currentPadState_.isConnected = false;
    }
    else
    {
        currentPadState_.isConnected = true;
    }

    if (!currentPadState_.isConnected) return;


    // 振動停止
    if (currentPadState_.isVibrating) return;
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

bool Glib::Internal::Input::XinputGamePad::GetButton(GPADKey button)
{
    if (!CheckConnection()) return false;
    auto type = static_cast<unsigned int>(button);
    return currentPadState_.state.Gamepad.wButtons & type;
}

bool Glib::Internal::Input::XinputGamePad::GetButtonDown(GPADKey button)
{
    if (!CheckConnection()) return false;
    auto type = static_cast<unsigned int>(button);
    return currentPadState_.state.Gamepad.wButtons & type &
        ~prevPadState_.state.Gamepad.wButtons & type;
}

bool Glib::Internal::Input::XinputGamePad::GetButtonUp(GPADKey button)
{
    if (!CheckConnection()) return false;
    auto type = static_cast<unsigned int>(button);
    return prevPadState_.state.Gamepad.wButtons & type &
        ~currentPadState_.state.Gamepad.wButtons & type;
}

Vector2 Glib::Internal::Input::XinputGamePad::GetLeftStick(float deadZone)
{
    if (!CheckConnection()) return Vector2::Zero();
    short x = currentPadState_.state.Gamepad.sThumbLX;
    short y = currentPadState_.state.Gamepad.sThumbLY;

    Vector2 stickVal
    {
        x / (x > 0 ? 32767.0f : 32768.0f),
        y / (y > 0 ? 32767.0f : 32768.0f),
    };

    return (stickVal.SqrMagnitude() < deadZone * deadZone) ?
        Vector2::Zero() :
        stickVal;
}

Vector2 Glib::Internal::Input::XinputGamePad::GetRightStick(float deadZone)
{
    if (!CheckConnection()) return Vector2::Zero();
    short x = currentPadState_.state.Gamepad.sThumbRX;
    short y = currentPadState_.state.Gamepad.sThumbRY;

    Vector2 stickVal
    {
        x / (x > 0 ? 32767.0f : 32768.0f),
        y / (y > 0 ? 32767.0f : 32768.0f),
    };

    return (stickVal.SqrMagnitude() < deadZone * deadZone) ?
        Vector2::Zero() :
        stickVal;
}

float Glib::Internal::Input::XinputGamePad::GetLeftTrigger(float deadZone)
{
    if (!CheckConnection()) return 0.0f;
    float triggerVal = currentPadState_.state.Gamepad.bLeftTrigger / 255.0f;
    return triggerVal < deadZone ? 0.0f : triggerVal;
}

float Glib::Internal::Input::XinputGamePad::GetRightTrigger(float deadZone)
{
    if (!CheckConnection()) return 0.0f;
    float triggerVal = currentPadState_.state.Gamepad.bRightTrigger / 255.0f;
    return triggerVal < deadZone ? 0.0f : triggerVal;
}

void Glib::Internal::Input::XinputGamePad::VibratePad(float left, float right, float time)
{
    if (!CheckConnection()) return;
    XINPUT_VIBRATION vibration{};
    vibration.wLeftMotorSpeed = static_cast<unsigned short>(left * 65535);
    vibration.wRightMotorSpeed = static_cast<unsigned short>(right * 65535);
    currentPadState_.isVibrating = true;
    currentPadState_.vibratingStartTime = GameTimer::Now();
    currentPadState_.vibratingTime = time;
    XInputSetState(controllerIndex_, &vibration);
}

bool Glib::Internal::Input::XinputGamePad::CheckConnection() const
{
    return currentPadState_.isConnected;
}
