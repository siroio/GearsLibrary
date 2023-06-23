#include <Window.h>
#include <Vector2.h>
#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif



namespace
{
    HINSTANCE instanceHandle_;
    HWND windowHandle_;
    Glib::WndProc windowProc_ = WndProc;
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

bool Glib::Window::Initialize()
{
    // 作成済みかチェック
    if (windowHandle_ != NULL) return false;
    if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED))) return false;

#ifdef _WIN32
#ifdef _DEBUG
    // メモリリーク検出
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

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

HINSTANCE Glib::Window::InstanceHandle()
{
    return instanceHandle_;
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
