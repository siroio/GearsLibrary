#pragma once
#include <Internal/DirectInput8.h>
#include <Internal/MouseButton.h>
#include <ComPtr.h>
#include <Vector2.h>

namespace Glib::Internal::Input
{
    class MouseDevice
    {
    public:
        MouseDevice() = default;
        ~MouseDevice();
        MouseDevice(const MouseDevice&) = delete;
        MouseDevice& operator = (const MouseDevice&) = delete;

        bool Initialize(ComPtr<IDirectInput8>& dinput);
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
        DIMOUSESTATE2 currentMouseState_{ NULL };
        DIMOUSESTATE2 prevMouseState_{ NULL };
        LPDIRECTINPUTDEVICE8 inputDevice_{ nullptr };
    };
}
