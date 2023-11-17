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
        bool Initialize(ComPtr<IDirectInput8>& dinput);
        void Update();

        /**
         * @brief キーが押されているか
        */
        bool GetKey(KeyCode key) const;

        /**
         * @brief キーが離されたか
        */
        bool GetKeyDown(KeyCode key) const;

        /**
         * @brief キーが押されたか
        */
        bool GetKeyUp(KeyCode key) const;
    private:
        ComPtr<IDirectInputDevice8> device_{ nullptr };
        std::array<unsigned char, 256U> prevKeyState{ 0U };
        std::array<unsigned char, 256U> currentKeyState{ 0U };
    };
}
