#include <Internal/MouseDevice.h>
#include <vector>

namespace
{
    enum KEY : unsigned char
    {
        DOWN = 0,
        UP = 1
    };
}

Glib::Internal::Input::MouseDevice::~MouseDevice()
{
    // デバイスの削除
    if (inputDevice_ != nullptr)
    {
        inputDevice_->Unacquire();
        inputDevice_->Release();
        inputDevice_ = nullptr;
    }
}

bool Glib::Internal::Input::MouseDevice::Initialize(ComPtr<IDirectInput8>& dinput)
{
    // デバイスの作成
    auto hr = dinput->CreateDevice(
        GUID_SysMouse,
        &inputDevice_,
        nullptr
    );
    if (FAILED(hr)) return false;

    // フォーマット設定
    hr = inputDevice_->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr)) return false;

    hr = inputDevice_->SetCooperativeLevel(
        GetActiveWindow(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
    );
    if (FAILED(hr)) return false;
    inputDevice_->Acquire();

    return true;
}

void Glib::Internal::Input::MouseDevice::Update()
{
    prevMouseState_ = currentMouseState_;
    if (FAILED(inputDevice_->GetDeviceState(sizeof(currentMouseState_), &currentMouseState_)))
    {
        inputDevice_->Acquire();
        inputDevice_->GetDeviceState(sizeof(currentMouseState_), &currentMouseState_);
    }
}

bool Glib::Internal::Input::MouseDevice::ButtonDown(MouseButton button) const
{
    return currentMouseState_.rgbButtons[static_cast<BYTE>(button)] & 0x80;
}

bool Glib::Internal::Input::MouseDevice::ButtonUP(MouseButton button) const
{
    return currentMouseState_.rgbButtons[static_cast<BYTE>(button)] & 0x8 &
        ~prevMouseState_.rgbButtons[static_cast<BYTE>(button)] & 0x8;
}

bool Glib::Internal::Input::MouseDevice::Pressed(MouseButton button) const
{
    return prevMouseState_.rgbButtons[static_cast<BYTE>(button)] & 0x8 &
        ~currentMouseState_.rgbButtons[static_cast<BYTE>(button)] & 0x8;
}

Vector2 Glib::Internal::Input::MouseDevice::Position() const
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);
    return Vector2{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
}

Vector2 Glib::Internal::Input::MouseDevice::Delta() const
{
    return Vector2{ static_cast<float>(currentMouseState_.lX), static_cast<float>(currentMouseState_.lY) };
}

float Glib::Internal::Input::MouseDevice::Wheel() const
{
    return currentMouseState_.lZ / 120.0f;
}

void Glib::Internal::Input::MouseDevice::ShowCursor() const
{
    ::ShowCursor(TRUE);
}

void Glib::Internal::Input::MouseDevice::HideCursor() const
{
    ::ShowCursor(FALSE);
}

void Glib::Internal::Input::MouseDevice::SetPosition(const Vector2& position) const
{
    POINT leftTop{ 0, 0 };
    ScreenToClient(GetActiveWindow(), &leftTop);
    SetCursorPos(static_cast<int>(position.x - leftTop.x), static_cast<int>(position.y - leftTop.y));
}
