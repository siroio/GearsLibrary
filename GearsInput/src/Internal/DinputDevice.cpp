#include <Internal/DinputDevice.h>

bool Glib::Internal::Input::DinputDevice::Initialize()
{
    if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)directInput_.ReleaseAndGetAddressOf(), nullptr)))
    {
        return false;
    }

    if (!InitializeDevice())
    {
        return false;
    }

    return true;
}

void Glib::Internal::Input::DinputDevice::Update()
{
    DIJOYSTATE joyState{};
    if (device_->GetDeviceState(sizeof(DIJOYSTATE), &joyState) == DI_OK)
    {

    }
}

bool Glib::Internal::Input::DinputDevice::InitializeDevice()
{
    if (FAILED(directInput_->CreateDevice(GUID_Joystick, device_.ReleaseAndGetAddressOf(), nullptr)))
    {
        return false;
    }

    if (FAILED(device_->SetDataFormat(&c_dfDIJoystick)))
    {

    }

    return true;
}
