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
         * @brief Windowの初期化
         */
        bool Initialize();

        /**
         * @brief インスタンスハンドルの取得
         */
        static HINSTANCE InstanceHandle();

        /**
         * @brief ウィンドウハンドルの取得
         */
        static HWND WindowHandle();

        /**
         * @brief ウィンドウプロシージャを設定
         * @param proc
         */
        static void WndProc(WndProc proc);

        /**
         * @brief ウィンドウの名前を取得
         */
        static std::string& WindowName();

        /**
         * @brief ウィンドウの名前を設定
         * @param name
         */
        static void WindowName(const std::string& name);

        /**
         * @brief ウィンドウのサイズを取得
         */
        static const Vector2& WindowSize();

        /**
         * @brief ウィンドウのサイズを設定
         * @param size
         */
        static void WindowSize(const Vector2& size);

        /**
         * @brief デバッグ時のウィンドウサイズを取得
         */
        static const Vector2& WindowDebugSize();

        /**
         * @brief デバッグ時のウィンドウサイズを設定
         */
        static void WindowDebugSize(const Vector2& size);
    };
}

#endif // !GEARS_WINDOW_H
