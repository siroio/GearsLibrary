#include <Internal/DinputGamePad.h>
#include <Vector2.h>
#include <Mathf.h>

bool Glib::Internal::Input::DinputGamePad::Initialize(ComPtr<IDirectInput8>& dinput)
{
    if (FAILED(dinput->CreateDevice(GUID_Joystick, device_.ReleaseAndGetAddressOf(), nullptr))) return false;
    if (FAILED(device_->SetDataFormat(&c_dfDIJoystick))) return false;
    if (FAILED(device_->SetCooperativeLevel(GetActiveWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;

    // プロパティを設定
    DIPROPDWORD diprop{};
    diprop.diph.dwSize = sizeof(DIPROPDWORD);
    diprop.diph.dwHeaderSize = sizeof(DIPROPDWORD);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.dwData = DIPROPAXISMODE_ABS;
    if (FAILED(device_->SetProperty(DIPROP_AXISMODE, &diprop.diph))) return false;

    // デバイスの列挙形式
    DIDEVCAPS devCaps{};
    devCaps.dwSize = sizeof(DIDEVCAPS);
    if (FAILED(device_->GetCapabilities(&devCaps))) return false;
    if (FAILED(device_->Acquire())) return false;

    return true;
}

void Glib::Internal::Input::DinputGamePad::Update()
{
    prevState_ = currentState_;
    // 状態を取得
    if (FAILED(device_->Poll())) return;

    if (FAILED(device_->GetDeviceState(sizeof(DIJOYSTATE), &currentState_))) return;
    // デバイスが無効になっている場合は再取得を試みる
    if (DIERR_INPUTLOST == device_->Acquire())
    {
        device_->Acquire();
        device_->GetDeviceState(sizeof(DIJOYSTATE), &currentState_);
    }
}
bool Glib::Internal::Input::DinputGamePad::GetButton(GPADKey button)
{
    return currentState_.rgbButtons[static_cast<unsigned int>(button)] & 0x80;
}

bool Glib::Internal::Input::DinputGamePad::GetButtonDown(GPADKey button)
{
    return (currentState_.rgbButtons[static_cast<unsigned int>(button)] & 0x80) &&
        !(prevState_.rgbButtons[static_cast<unsigned int>(button)] & 0x80);
}

bool Glib::Internal::Input::DinputGamePad::GetButtonUp(GPADKey button)
{
    return !(currentState_.rgbButtons[static_cast<unsigned int>(button)] & 0x80) &&
        (prevState_.rgbButtons[static_cast<unsigned int>(button)] & 0x80);
}

Vector2 Glib::Internal::Input::DinputGamePad::GetLeftStick(float deadZone)
{
    return GetStickPosition(currentState_.lX, currentState_.lY, deadZone);
}

Vector2 Glib::Internal::Input::DinputGamePad::GetRightStick(float deadZone)
{
    return GetStickPosition(currentState_.lZ, currentState_.lRz, deadZone);
}

float Glib::Internal::Input::DinputGamePad::GetLeftTrigger(float deadZone)
{
    return GetTriggerValue(currentState_.rglSlider[0], deadZone);
}

float Glib::Internal::Input::DinputGamePad::GetRightTrigger(float deadZone)
{
    return GetTriggerValue(currentState_.rglSlider[1], deadZone);
}

Vector2 Glib::Internal::Input::DinputGamePad::GetStickPosition(long xAxis, long yAxis, float deadZone)
{
    float normX = static_cast<float>(xAxis) / 1000.0f;
    float normY = static_cast<float>(yAxis) / 1000.0f;
    float magnitude = Mathf::Sqrt(normX * normX + normY * normY);

    if (magnitude < deadZone)
    {
        return Vector2::Zero();
    }

    float normalized = (magnitude - deadZone) / (1.0f - deadZone);
    return Vector2{ normX / magnitude * normalized, normY / magnitude * normalized };
}

float Glib::Internal::Input::DinputGamePad::GetTriggerValue(long triggerValue, float deadZone)
{
    float normTrigger = static_cast<float>(triggerValue) / 1000.0f;
    if (normTrigger < deadZone)
    {
        return 0.0f;
    }
    return (normTrigger - deadZone) / (1.0f - deadZone);
}
