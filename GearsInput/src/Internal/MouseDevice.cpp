#include <Internal/MouseDevice.h>
#include <BitFlag.h>
#include <WinUser.h>
#include <vector>

Glib::Internal::Input::MouseDevice::~MouseDevice()
{
    rawInputDevice_->dwFlags = RIDEV_REMOVE;
    RegisterRawInputDevices(rawInputDevice_.get(), 1, sizeof(RAWINPUTDEVICE));
    rawInputDevice_.reset();

    Window::UnRegisterProcedure(this);
}

bool Glib::Internal::Input::MouseDevice::Initialize()
{
    // キーボード取得のデバイスを作成
    rawInputDevice_ = std::make_unique<RAWINPUTDEVICE>(0x01, 0x02, 0, nullptr);
    // デバイスの登録
    if (!RegisterRawInputDevices(rawInputDevice_.get(), 1, sizeof(RAWINPUTDEVICE))) return false;

    // メッセージ取得用のプロシージャ登録
    Window::RegisterProcedure(this);

    return true;
}

void Glib::Internal::Input::MouseDevice::Update()
{
    prevMouseBuffer_ = currentMouseBuffer_;

    // バッファから取得
    if (!frameBuffer_.empty())
    {
        for (const auto& buffer : frameBuffer_)
        {
            // unroll
            currentMouseBuffer_.Buttons[0] = buffer.Buttons[0];
            currentMouseBuffer_.Buttons[1] = buffer.Buttons[1];
            currentMouseBuffer_.Buttons[2] = buffer.Buttons[2];
            currentMouseBuffer_.Buttons[3] = buffer.Buttons[3];
            currentMouseBuffer_.Buttons[4] = buffer.Buttons[4];
        }
        currentMouseBuffer_.Position = frameBuffer_.back().Position;
        currentMouseBuffer_.Wheel = frameBuffer_.back().Wheel;
        currentMouseBuffer_.Delta = currentMouseBuffer_.Position - prevMouseBuffer_.Position;
        frameBuffer_.clear();
    }
}

bool Glib::Internal::Input::MouseDevice::ButtonDown(MouseButton button) const
{
    bool prev = prevMouseBuffer_.Buttons[static_cast<int>(button)];
    bool current = currentMouseBuffer_.Buttons[static_cast<int>(button)];
    return !prev && current;
}

bool Glib::Internal::Input::MouseDevice::ButtonUP(MouseButton button) const
{
    bool prev = prevMouseBuffer_.Buttons[static_cast<int>(button)];
    bool current = currentMouseBuffer_.Buttons[static_cast<int>(button)];
    return prev && !current;
}

bool Glib::Internal::Input::MouseDevice::Pressed(MouseButton button) const
{
    return currentMouseBuffer_.Buttons[static_cast<int>(button)];
}

Vector2 Glib::Internal::Input::MouseDevice::Position() const
{
    return currentMouseBuffer_.Position;
}

Vector2 Glib::Internal::Input::MouseDevice::Delta() const
{
    return currentMouseBuffer_.Delta;
}

float Glib::Internal::Input::MouseDevice::Wheel() const
{
    return currentMouseBuffer_.Wheel;
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

void Glib::Internal::Input::MouseDevice::ProcessMouse(const HRAWINPUT* hRawInput)
{
    UINT size = 0;
    GetRawInputData(*hRawInput, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
    if (size <= 0) return;
    auto buffer = std::make_unique<BYTE[]>(size);
    if (GetRawInputData(*hRawInput, RID_INPUT, buffer.get(), &size, sizeof(RAWINPUTHEADER)) != size)
    {
        return;
    }

    // バッファに追加
    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.get());
    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        auto mouse = raw->data.mouse;
        auto flags = raw->data.mouse.usFlags;
        auto buttonFlags = raw->data.mouse.usButtonFlags;
        MouseBuffer buffer;

        // 位置情報の取得
        if (flags & MOUSE_MOVE_ABSOLUTE)
        {
            // 仮想デスクトップか確認
            RECT rect{};
            if (flags & MOUSE_VIRTUAL_DESKTOP)
            {
                rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
                rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
                rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
                rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
            }
            else
            {
                rect.left = 0;
                rect.top = 0;
                rect.right = GetSystemMetrics(SM_CXSCREEN);
                rect.bottom = GetSystemMetrics(SM_CYSCREEN);
            }

            // 位置を計算
            buffer.Position = Vector2{
                static_cast<float>(MulDiv(mouse.lLastX, rect.right, 65535) + rect.left),
                static_cast<float>(MulDiv(mouse.lLastY, rect.bottom, 65535) + rect.top)
            };
        }
        else if (mouse.lLastX != 0 || mouse.lLastY != 0)
        {
            POINT currentPos{};
            GetCursorPos(&currentPos);
            ScreenToClient(GetActiveWindow(), &currentPos);
            buffer.Position = Vector2{
                static_cast<float>(currentPos.x + mouse.lLastX),
                static_cast<float>(currentPos.y + mouse.lLastY)
            };
        }

        // マウスホイールの取得
        if (buttonFlags & RI_MOUSE_WHEEL)
        {
            float delta = static_cast<float>(raw->data.mouse.usButtonData);
            buffer.Wheel = delta / WHEEL_DELTA;
        }

        // ボタンを確認
        buffer.Buttons[0] = buttonFlags & RI_MOUSE_BUTTON_1_DOWN;
        buffer.Buttons[1] = buttonFlags & RI_MOUSE_BUTTON_2_DOWN;
        buffer.Buttons[2] = buttonFlags & RI_MOUSE_BUTTON_3_DOWN;
        buffer.Buttons[3] = buttonFlags & RI_MOUSE_BUTTON_4_DOWN;
        buffer.Buttons[4] = buttonFlags & RI_MOUSE_BUTTON_5_DOWN;

        // バッファに追加
        frameBuffer_.push_back(std::move(buffer));
    }
}

void Glib::Internal::Input::MouseDevice::operator()(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg != WM_INPUT) return;
    auto inputData = reinterpret_cast<HRAWINPUT>(lparam);
    ProcessMouse(&inputData);
}
