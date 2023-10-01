#include <Window.h>
#include <Vector2.h>
#include <unordered_set>
#include <Internal/DX12/DirectX12.h>

namespace
{
    WNDCLASSEX s_windowClass_;
    HINSTANCE s_hInstance_;
    HWND s_windowHandle_;
    std::unordered_set<Glib::WndProc> s_windowProcedures;
    std::string s_windowName_{ "GameWindow" };
    Vector2 s_windowSize_{ 1240.0f, 720.0f };
    Vector2 s_windowDebugSize_{ 1240.0f, 720.0f };
    bool s_isFullScreen{ false };
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    for (auto&& proc : s_windowProcedures)
    {
        std::invoke(proc, hwnd, msg, wparam, lparam);
    }

    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            CoUninitialize();
            break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}

bool Glib::Window::Initialize()
{
    // 作成済みかチェック
    if (s_windowHandle_ != NULL) return false;

    // Comの初期化
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) return false;

    // ウィンドウの作成
    s_windowClass_.cbSize = sizeof(WNDCLASSEX);
    s_windowClass_.hInstance = s_hInstance_ = GetModuleHandle(nullptr);
    s_windowClass_.lpfnWndProc = (WNDPROC)WindowProcedure;
    auto style = WS_OVERLAPPEDWINDOW;

#ifdef UNICODE
    std::wstring wstr{ s_windowName_.begin(), s_windowName_.end() };
    s_windowClass_.lpszClassName = wstr.c_str();
#else
    s_windowClass_.lpszClassName = s_windowName_.c_str();
#endif

#if defined(DEBUG) || defined(_DEBUG)
    RECT rect{ 0, 0, static_cast<LONG>(s_windowDebugSize_.x), static_cast<LONG>(s_windowDebugSize_.y) };
#else
    RECT rect{ 0, 0, static_cast<LONG>(s_windowSize_.x), static_cast<LONG>(s_windowSize_.y) };
#endif

    if (!RegisterClassEx(&s_windowClass_)) return false;
    AdjustWindowRect(&rect, style, false);

    // ウィンドウ作成
    s_windowHandle_ = CreateWindowEx(
        0,
        s_windowClass_.lpszClassName,
        s_windowClass_.lpszClassName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        s_hInstance_,
        nullptr
    );

    if (s_windowHandle_ == nullptr) return false;

    ShowWindow(s_windowHandle_, SW_SHOW);
    UpdateWindow(s_windowHandle_);
    SetFocus(s_windowHandle_);

    return true;
}

void Glib::Window::Finalize()
{
    UnregisterClass(s_windowClass_.lpszClassName, s_hInstance_);
    s_hInstance_ = nullptr;
    s_windowHandle_ = nullptr;
}

HINSTANCE Glib::Window::InstanceHandle()
{
    return s_hInstance_;
}

HWND Glib::Window::WindowHandle()
{
    return s_windowHandle_;
}

void Glib::Window::WndProc(const Glib::WndProc& proc)
{
    s_windowProcedures.emplace(proc);
}

void Glib::Window::ClearWndProc(const Glib::WndProc& proc)
{
    if (!s_windowProcedures.contains(proc)) return;
    s_windowProcedures.erase(proc);
}

std::string& Glib::Window::WindowName()
{
    return s_windowName_;
}

void Glib::Window::WindowName(const std::string& name)
{
    if (s_windowHandle_ != NULL) return;
    s_windowName_ = name;
}

const Vector2& Glib::Window::WindowSize()
{
    return s_windowSize_;
}

void Glib::Window::WindowSize(const Vector2& size)
{
    if (s_windowHandle_ != NULL) return;
    s_windowSize_ = size;
}

const Vector2& Glib::Window::WindowDebugSize()
{
    return s_windowDebugSize_;
}

void Glib::Window::WindowDebugSize(const Vector2& size)
{
    if (s_windowHandle_ != NULL) return;
    s_windowDebugSize_ = size;
}
