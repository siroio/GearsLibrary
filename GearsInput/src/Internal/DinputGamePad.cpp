#pragma once
#include <Internal/DinputGamePad.h>

bool Glib::Internal::Input::DinputGamePad::Initialize(ComPtr<IDirectInput8> dinput)
{
    if (FAILED(dinput->CreateDevice(GUID_Joystick, device_.ReleaseAndGetAddressOf(), nullptr))) return false;
    if (FAILED(device_->SetDataFormat(&c_dfDIJoystick))) return false;
    if (FAILED(device_->SetCooperativeLevel(GetActiveWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;
    if (FAILED(device_->Acquire())) return false;
    return true;
}

void Glib::Internal::Input::DinputGamePad::Update()
{
    DIJOYSTATE joyState{};
    if (FAILED(device_->GetDeviceState(sizeof(DIJOYSTATE), &joyState)))
    {
        auto hr = device_->Acquire();
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
            device_->Acquire();
        device_->GetDeviceState(sizeof(DIJOYSTATE), &joyState);
    }
}
