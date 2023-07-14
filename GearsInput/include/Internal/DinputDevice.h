#pragma once

#include <Windows.h>
#include <dinput.h>
#include <ComPtr.h>
#include <array>

namespace Glib::Internal::Input
{
    class DinputDevice
    {
    public:
        bool Initialize();
        void Update();

    private:
        bool InitializeDevice();

    private:
        ComPtr<IDirectInput8> directInput_;
        ComPtr<IDirectInputDevice8> device_;
        std::array<unsigned char, 128> button_;
        std::array<long, 8> axisPosition_;
    };
}
