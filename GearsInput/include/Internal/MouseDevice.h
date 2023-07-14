#pragma once
#include <Windows.h>
#include <Vector2.h>
#include <Internal/MouseButton.h>
#include <unordered_map>

namespace Glib::Internal::Input
{
    class MouseDevice
    {
    public:
        ~MouseDevice();
        bool Initialize();
        void Update();

        bool ButtonDown(MouseButton button);
        bool ButtonUP(MouseButton button);
        bool Pressed(MouseButton button);

        Vector2 Position();
        Vector2 Delta();
    private:
        RAWINPUTDEVICE device_;
        Vector2 delta_;
        Vector2 position_;
        std::unordered_map<MouseButton, std::array<USHORT, 2>> prevMouseButton_;
        std::unordered_map<MouseButton, std::array<USHORT, 2>> currentMouseButton_;
    };
}