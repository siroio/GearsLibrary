#pragma once
#include <Internal/KeyBoardKeys.h>
#include <Window.h>
#include <memory>
#include <array>
#include <unordered_map>

namespace Glib::Internal::Input
{
    class KeyBoardDevice : public IWindowMessage
    {
    public:
        ~KeyBoardDevice();
        bool Initialize();
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
        void ProcessKeyboard(const HRAWINPUT* hRawInput);
        void operator()(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

    private:
        std::array<unsigned char, 256U> prevKeyState_{ 0U };
        std::array<unsigned char, 256U> currentKeyState_{ 0U };
        std::unordered_map<unsigned short, bool> frameBuffer_;
        std::unique_ptr<RAWINPUTDEVICE> rawInputDevice_;
    };
}
