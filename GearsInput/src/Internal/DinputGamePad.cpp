#pragma once
#include <Internal/DinputGamePad.h>

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

    button_.fill(0);
    return true;
}

void Glib::Internal::Input::DinputGamePad::Update()
{
    // 状態を取得
    DIJOYSTATE joyState{};
    if (FAILED(device_->Poll()))
    {
        // デバイスが無効になっている場合は再取得を試みる
        while (DIERR_INPUTLOST == device_->Acquire())
        {
            device_->Acquire();
        }
        return;
    }

    if (FAILED(device_->GetDeviceState(sizeof(DIJOYSTATE), &joyState))) return;

    for (size_t i = 0; i < button_.size(); i++)
    {
        button_[i] = joyState.rgbButtons[i] & 0x80;
    }

    // 軸の値を更新
    axisX_ = static_cast<float>(joyState.lX) / 1000.0f;
    axisY_ = static_cast<float>(joyState.lY) / 1000.0f;
}

