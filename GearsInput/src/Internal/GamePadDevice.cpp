#include <Internal/GamePadDevice.h>

bool Glib::Internal::Input::GamePadDevice::Initialize(ComPtr<IDirectInput8> dinput)
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
        std::visit([](auto&& dev)
        {
            dev.Update();
        }, device);
    }
}
