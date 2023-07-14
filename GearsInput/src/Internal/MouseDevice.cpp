#include <Internal/MouseDevice.h>
#include <vector>
namespace
{
    enum KEY
    {
        DOWN = 0,
        UP = 1
    };
}

bool Glib::Internal::Input::MouseDevice::Initialize()
{
    // デバイスの登録
    return true;
}

void Glib::Internal::Input::MouseDevice::Update()
{

}

bool Glib::Internal::Input::MouseDevice::ButtonDown(MouseButton button)
{
    return false;
}

bool Glib::Internal::Input::MouseDevice::ButtonUP(MouseButton button)
{
    return false;
}

bool Glib::Internal::Input::MouseDevice::Pressed(MouseButton button)
{
    return false;
}

Vector2 Glib::Internal::Input::MouseDevice::Position()
{
    return Vector2{};
}

Vector2 Glib::Internal::Input::MouseDevice::Delta()
{
    return Vector2{};
}

Glib::Internal::Input::MouseDevice::~MouseDevice()
{
    // デバイスの解除

}
