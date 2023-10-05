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
    // �f�o�C�X�̓o�^
    device_.usUsagePage = 1;
    device_.usUsage = 2;
    device_.dwFlags = 0;
    device_.hwndTarget = window.WindowHandle();
    RegisterRawInputDevices(&device_, 1, sizeof(RAWINPUTDEVICE));
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

void Glib::Internal::Input::MouseDevice::RawInputMsgHandler(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg != WM_INPUT) return;
    // �f�[�^�T�C�Y�̎擾
    UINT dataSize{ 0 };
    GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));

    if (dataSize > 0) return;
    // �f�[�^�̎擾
    std::vector<BYTE> rawInputData(dataSize);
    bool failed = GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lparam),
        RID_INPUT,
        rawInputData.data(),
        &dataSize,
        sizeof(RAWINPUTHEADER)) == 1;

    if (failed) return;
    auto rawInput = reinterpret_cast<RAWINPUT*>(rawInputData.data());
    if (rawInput->header.dwType != RIM_TYPEMOUSE) return;
    delta_.x = static_cast<float>(rawInput->data.mouse.lLastX);
    delta_.y = static_cast<float>(rawInput->data.mouse.lLastY);
    position_.x = static_cast<float>(rawInput->data.mouse.lLastX);
    position_.y = static_cast<float>(rawInput->data.mouse.lLastY);

    for (int i = 0; i < 5; i++)
    {
        currentMouseButton_[i] = (rawInput->data.mouse.usButtonFlags >> (i * 2)) & 1 ? KEY::DOWN : KEY::UP;
    }
}

Glib::Internal::Input::MouseDevice::~MouseDevice()
{
    // �f�o�C�X�̉���

}
