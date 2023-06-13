#include <Window.h>
#include <Vector2.h>

namespace
{
    HINSTANCE instanceHandle_;
    HWND windowHandle_;
    GLib::WndProc windowProc_ = WndProc;
    std::string windowName_{ "GameWindow" };
    Vector2 windowSize_{ 1240.0f, 720.0f };
    Vector2 windowDebugSize_{ 1240.0f, 720.0f };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            CoUninitialize();
            break;
        default:
            return(DefWindowProc(hwnd, msg, wParam, lParam));
    }
    return 0;
}

bool GLib::Window::Initialize()
{
    // 作成済みかチェック
    if (windowHandle_ != NULL) return false;
    if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED))) return false;

    WNDCLASSEX wc{};
    wc.hInstance = instanceHandle_ = GetModuleHandle(nullptr);
    wc.lpfnWndProc = (WNDPROC)windowProc_;
#ifdef UNICODE
    std::wstring wstr{ windowName_.begin(), windowName_.end() };
    wc.lpszClassName = wstr.c_str();
#else
    wc.lpszClassName windowName_.c_str();
#endif

    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

#ifdef _DEBUG
    RECT rect{ 0, 0, static_cast<LONG>(windowDebugSize_.x), static_cast<LONG>(windowDebugSize_.y) };
#else
    RECT rect{ 0, 0, static_cast<LONG>(windowSize_.x), static_cast<LONG>(windowSize_.y) };
#endif

    RegisterClassEx(&wc);
    AdjustWindowRect(&rect, style, false);

    // ウィンドウ作成
    windowHandle_ = CreateWindow(
        wc.lpszClassName,
        wc.lpszClassName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        instanceHandle_,
        nullptr
    );

    if (windowHandle_ == NULL) return false;
    ShowWindow(windowHandle_, SW_SHOW);
    return true;
}

HINSTANCE GLib::Window::InstanceHandle()
{
    return instanceHandle_;
}

HWND GLib::Window::WindowHandle()
{
    return windowHandle_;
}

void GLib::Window::WndProc(GLib::WndProc proc)
{
    windowProc_ = proc;
}

std::string& GLib::Window::WindowName()
{
    return windowName_;
}

void GLib::Window::WindowName(const std::string& name)
{
    if (windowHandle_ != NULL) return;
    windowName_ = name;
}

const Vector2& GLib::Window::WindowSize()
{
    return windowSize_;
}

void GLib::Window::WindowSize(const Vector2& size)
{
    if (windowHandle_ != NULL) return;
    windowSize_ = size;
}

const Vector2& GLib::Window::WindowDebugSize()
{
    return windowDebugSize_;
}

void GLib::Window::WindowDebugSize(const Vector2& size)
{
    if (windowHandle_ != NULL) return;
    windowDebugSize_ = size;
}
