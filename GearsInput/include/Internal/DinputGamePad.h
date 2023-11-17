#pragma once
// DINPUTバージョン指定
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include <ComPtr.h>
#include <array>

namespace Glib::Internal::Input
{
    class DinputGamePad
    {
    public:
        bool Initialize(ComPtr<IDirectInput8>& dinput);
        void Update();

    private:
        ComPtr<IDirectInputDevice8> device_{ nullptr };
        std::array<bool, 16> button_{ 0 };
        float axisX_{ 0.0f };
        float axisY_{ 0.0f };
    };
}
