#pragma once
#include <Windows.h>
#include <Singleton.h>
#include <string>
#include <functional>

struct Vector2;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace Glib
{
    using WindowProcedure = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

    class Window : public Singleton<Window>
    {
        Window() = default;
        friend Window& Glib::Singleton<Window>::Instance();

    public:

        /**
         * @brief Windowの初期化
         */
        bool Initialize();

        /**
         * @brief Windowの終了
        */
        void Finalize();

    public:

        /**
         * @brief インスタンスハンドルの取得
         */
        static HINSTANCE InstanceHandle();

        /**
         * @brief ウィンドウハンドルの取得
         */
        static HWND WindowHandle();

        /**
         * @brief ウィンドウプロシージャを登録
         * @param proc プロシージャ
         * @param id ID
         */
        static int RegisterProcedure(const Glib::WindowProcedure& proc, int id = -1);

        /**
         * @brief ウィンドウプロシージャを解除
         * @param proc
         */
        static void UnRegisterProcedure(int id);

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
