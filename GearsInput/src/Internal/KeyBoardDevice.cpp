#include <Internal/KeyBoardDevice.h>
#include <Logger.h>

bool Glib::Internal::Input::KeyBoardDevice::Initialize(ComPtr<IDirectInput8> dinput)
{
    if (FAILED(dinput->CreateDevice(GUID_SysKeyboard, device_.ReleaseAndGetAddressOf(), nullptr))) return false;
    if (FAILED(device_->SetDataFormat(&c_dfDIKeyboard))) return false;
    if (FAILED(device_->SetCooperativeLevel(GetActiveWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;
    if (FAILED(device_->Acquire())) return false;

    currentKeyState.fill(0U);
    prevKeyState.fill(0U);
    return true;
}

void Glib::Internal::Input::KeyBoardDevice::Update()
{
    prevKeyState = currentKeyState;
    if (FAILED(device_->GetDeviceState(256, currentKeyState.data())))
    {
        auto hr = device_->Acquire();
        device_->GetDeviceState(256, currentKeyState.data());
    }
}

bool Glib::Internal::Input::KeyBoardDevice::GetKey(KeyCode key) const
{
    return currentKeyState.at(static_cast<unsigned char>(key)) & 0x80;
}

bool Glib::Internal::Input::KeyBoardDevice::GetKeyDown(KeyCode key) const
{
    unsigned char prevState = ~(prevKeyState.at(static_cast<unsigned char>(key)) & 0x80);
    unsigned char currentState = currentKeyState.at(static_cast<unsigned char>(key)) & 0x80;
    return prevState & currentState;
}

bool Glib::Internal::Input::KeyBoardDevice::GetKeyUp(KeyCode key) const
{
    unsigned char prevState = prevKeyState.at(static_cast<unsigned char>(key)) & 0x80;
    unsigned char currentState = ~(currentKeyState.at(static_cast<unsigned char>(key)) & 0x80);
    return prevState & currentState;
}
