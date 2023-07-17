#pragma once
#include <dinput.h>
#include <ComPtr.h>

namespace Glib::Internal::Input
{
    class DinputGamePad
    {
    public:
        bool Initialize(ComPtr<IDirectInput8> dinput);
        void Update();

    private:
        ComPtr<IDirectInputDevice8> device_;
    };
}