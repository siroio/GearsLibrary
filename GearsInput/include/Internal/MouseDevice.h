#pragma once
#include <Internal/DirectInput8.h>
#include <Internal/MouseButton.h>
#include <Window.h>
#include <Vector2.h>
#include <memory>
#include <array>
#include <deque>

namespace Glib::Internal::Input
{
    class MouseDevice : public IWindowMessage
    {
    private:
        struct MouseBuffer
        {
            Vector2 Position;
            Vector2 Delta;
            float Wheel;
            std::array<bool, 5> Buttons;
        };

    public:
        ~MouseDevice();
        bool Initialize();
        void Update();

        bool ButtonDown(MouseButton button) const;
        bool ButtonUP(MouseButton button) const;
        bool Pressed(MouseButton button) const;
        Vector2 Position() const;
        Vector2 Delta() const;
        float Wheel() const;
        void ShowCursor() const;
        void HideCursor() const;
        void SetPosition(const Vector2& position) const;

    private:
        void ProcessMouse(const HRAWINPUT* hRawInput);
        void operator()(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

    private:
        MouseBuffer prevMouseBuffer_;
        MouseBuffer currentMouseBuffer_;
        std::deque<MouseBuffer> frameBuffer_;
        std::unique_ptr<RAWINPUTDEVICE> rawInputDevice_;
    };
}
