#ifndef GEARS_WINDOW_H
#define GEARS_WINDOW_H

#include <windows.h>
#include <string>
#include <Singleton.h>

struct Vector2;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Glib
{
    using WndProc = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);

    class Window : public Singleton<Window>
    {
        Window() = default;
        friend Window& Singleton<Window>::Instance();

    public:

        /**
         * @brief Window�̏�����
         */
        bool Initialize();

        /**
         * @brief �C���X�^���X�n���h���̎擾
         */
        static HINSTANCE InstanceHandle();

        /**
         * @brief �E�B���h�E�n���h���̎擾
         */
        static HWND WindowHandle();

        /**
         * @brief �E�B���h�E�v���V�[�W����ݒ�
         * @param proc
         */
        static void WndProc(WndProc proc);

        /**
         * @brief �E�B���h�E�̖��O���擾
         */
        static std::string& WindowName();

        /**
         * @brief �E�B���h�E�̖��O��ݒ�
         * @param name
         */
        static void WindowName(const std::string& name);

        /**
         * @brief �E�B���h�E�̃T�C�Y���擾
         */
        static const Vector2& WindowSize();

        /**
         * @brief �E�B���h�E�̃T�C�Y��ݒ�
         * @param size
         */
        static void WindowSize(const Vector2& size);

        /**
         * @brief �f�o�b�O���̃E�B���h�E�T�C�Y���擾
         */
        static const Vector2& WindowDebugSize();

        /**
         * @brief �f�o�b�O���̃E�B���h�E�T�C�Y��ݒ�
         */
        static void WindowDebugSize(const Vector2& size);
    };
}

#endif // !GEARS_WINDOW_H
