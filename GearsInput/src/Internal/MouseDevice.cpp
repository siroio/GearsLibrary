#include <Internal/MouseDevice.h>
#include <Window.h>
#include <vector>

namespace
{
    enum KEY : unsigned char
    {
        DOWN = 0,
        UP = 1
    };

    auto& window = Glib::Window::Instance();
}

bool Glib::Internal::Input::MouseDevice::Initialize()
{
    // デバイスの登録
    window.WndProc([](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lpram)
    {
        return static_cast<LRESULT>(0);
    });
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
