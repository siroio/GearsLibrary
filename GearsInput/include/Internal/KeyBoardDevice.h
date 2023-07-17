#pragma once
#include <dinput.h>
#include <array>
#include <ComPtr.h>
#include<Internal/KeyBoardKeys.h>

namespace Glib::Internal::Input
{
    class KeyBoardDevice
    {
    public:
        bool Initialize(ComPtr<IDirectInput8> dinput);
        void Update();

        /**
         * @brief キーが押されているか
        */
        bool GetKey(KeyCode key);

        /**
         * @brief キーが離されたか
        */
        bool GetKeyDown(KeyCode key);

        /**
         * @brief キーが押されたか
        */
        bool GetKeyUp(KeyCode key);
    private:
        ComPtr<IDirectInputDevice8> device_{ nullptr };
        std::array<unsigned char, 256U> prevKeyState;
        std::array<unsigned char, 256U> currentKeyState;
    };
}