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
         * @brief �L�[��������Ă��邩
        */
        bool GetKey(KeyCode key);

        /**
         * @brief �L�[�������ꂽ��
        */
        bool GetKeyDown(KeyCode key);

        /**
         * @brief �L�[�������ꂽ��
        */
        bool GetKeyUp(KeyCode key);
    private:
        ComPtr<IDirectInputDevice8> device_{ nullptr };
        std::array<unsigned char, 256U> prevKeyState;
        std::array<unsigned char, 256U> currentKeyState;
    };
}