#include <Internal/GamePadDevice.h>
#include <Vector2.h>

bool Glib::Internal::Input::GamePadDevice::Initialize(ComPtr<IDirectInput8>& dinput)
{
    for (DWORD i = 0; i < 4; i++)
    {
        auto xGamePad = XinputGamePad{};
        auto dGamePad = DinputGamePad{};
        if (xGamePad.Initialize(i))
        {
            devices_[i] = std::move(xGamePad);
            return true;
        }
        else if (dGamePad.Initialize(dinput))
        {
            devices_[i] = std::move(dGamePad);
            return true;
        }
    }
    return false;
}

void Glib::Internal::Input::GamePadDevice::Update()
{
    for (auto& device : devices_)
    {
        std::visit([](auto& dev)
        {
            dev.Update();
        }, device);
    }
}

bool Glib::Internal::Input::GamePadDevice::GetButton(PadNum pad, GPADKey button)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&button](auto& dev)
    {
        return dev.GetButton(button);
    }, device);
}

bool Glib::Internal::Input::GamePadDevice::GetButtonDown(PadNum pad, GPADKey button)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&button](auto& dev)
    {
        return dev.GetButtonDown(button);
    }, device);
}

bool Glib::Internal::Input::GamePadDevice::GetButtonUp(PadNum pad, GPADKey button)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&button](auto& dev)
    {
        return dev.GetButtonUp(button);
    }, device);
}

Vector2 Glib::Internal::Input::GamePadDevice::GetLeftStick(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&deadZone](auto& dev)
    {
        return dev.GetLeftStick(deadZone);
    }, device);
}

Vector2 Glib::Internal::Input::GamePadDevice::GetRightStick(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&deadZone](auto& dev)
    {
        return dev.GetRightStick(deadZone);
    }, device);
}

float Glib::Internal::Input::GamePadDevice::GetLeftTrigger(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&deadZone](auto& dev)
    {
        return dev.GetLeftTrigger(deadZone);
    }, device);
}

float Glib::Internal::Input::GamePadDevice::GetRightTrigger(PadNum pad, float deadZone)
{
    auto idx = static_cast<unsigned int>(pad);
    auto& device = devices_.at(idx);
    return std::visit([&deadZone](auto& dev)
    {
        return dev.GetRightTrigger(deadZone);
    }, device);
}

void Glib::Internal::Input::GamePadDevice::VibrateGamepad(PadNum pad, float left, float right, float time)
{}
