#include <Window.h>
#include <Vector2.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif

namespace
{
    HINSTANCE hInstance_;
    HWND windowHandle_;
    Glib::WndProc windowProc_ = WindowProcedure;
    std::string windowName_{ "GameWindow" };
    Vector2 windowSize_{ 1240.0f, 720.0f };
    Vector2 windowDebugSize_{ 1240.0f, 720.0f };
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            CoUninitialize();
            break;
        default:
            return(DefWindowProc(hwnd, msg, wparam, lparam));
    }
    return 0;
}

bool Glib::Window::Initialize()
{
    // 作成済みかチェック
    if (windowHandle_ != NULL) return false;
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) return false;
#ifdef _DEBUG
    // メモリリーク検出
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInstance_ = GetModuleHandle(nullptr);
    wc.lpfnWndProc = (WNDPROC)windowProc_;
    // ウィンドウの名前
#ifdef UNICODE
    std::wstring wstr{ windowName_.begin(), windowName_.end() };
    wc.lpszClassName = wstr.c_str();
#else
    wc.lpszClassName = windowName_.c_str();
#endif

#ifdef _DEBUG
    RECT rect{ 0, 0, static_cast<LONG>(windowDebugSize_.x), static_cast<LONG>(windowDebugSize_.y) };
#else
    RECT rect{ 0, 0, static_cast<LONG>(windowSize_.x), static_cast<LONG>(windowSize_.y) };
#endif

    RegisterClassEx(&wc);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウ作成
    windowHandle_ = CreateWindow(
        wc.lpszClassName,
        wc.lpszClassName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance_,
        nullptr
    );

    if (windowHandle_ == NULL) return false;
    ShowWindow(windowHandle_, SW_SHOW);
    return true;
}

HINSTANCE Glib::Window::InstanceHandle()
{
    return hInstance_;
}

HWND Glib::Window::WindowHandle()
{
    return windowHandle_;
}

void Glib::Window::WndProc(Glib::WndProc proc)
{
    windowProc_ = proc;
}

std::string& Glib::Window::WindowName()
{
    return windowName_;
}

void Glib::Window::WindowName(const std::string& name)
{
    if (windowHandle_ != NULL) return;
    windowName_ = name;
}

const Vector2& Glib::Window::WindowSize()
{
    return windowSize_;
}

void Glib::Window::WindowSize(const Vector2& size)
{
    if (windowHandle_ != NULL) return;
    windowSize_ = size;
}

const Vector2& Glib::Window::WindowDebugSize()
{
    return windowDebugSize_;
}

void Glib::Window::WindowDebugSize(const Vector2& size)
{
    if (windowHandle_ != NULL) return;
    windowDebugSize_ = size;
}
