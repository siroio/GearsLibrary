#include <Internal/GamePadDevice.h>
#include <Vector2.h>

bool Glib::Internal::Input::GamePadDevice::Initialize()
{
    for (DWORD i = 0; i < static_cast<DWORD>(PadNum::CAPACITY); i++)
    {

        auto xGamePad = std::make_unique<XinputGamePad>();
        if (!xGamePad->Initialize(i)) return false;
        devices_[i] = std::move(xGamePad);
    }
    return true;
}

void Glib::Internal::Input::GamePadDevice::Update()
{
    for (auto& device : devices_)
    {
        device->Update();
    }
}

bool Glib::Internal::Input::GamePadDevice::GetButton(PadNum pad, GPADKey button)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetButton(button);
}

bool Glib::Internal::Input::GamePadDevice::GetButtonDown(PadNum pad, GPADKey button)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetButtonDown(button);
}

bool Glib::Internal::Input::GamePadDevice::GetButtonUp(PadNum pad, GPADKey button)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetButtonUp(button);
}

Vector2 Glib::Internal::Input::GamePadDevice::GetLeftStick(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetLeftStick(deadZone);
}

Vector2 Glib::Internal::Input::GamePadDevice::GetRightStick(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetRightStick(deadZone);
}

float Glib::Internal::Input::GamePadDevice::GetLeftTrigger(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetLeftTrigger(deadZone);
}

float Glib::Internal::Input::GamePadDevice::GetRightTrigger(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return device->GetRightTrigger(deadZone);
}

void Glib::Internal::Input::GamePadDevice::VibrateGamepad(PadNum pad, float left, float right, float time)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    device->VibratePad(left, right, time);
}
