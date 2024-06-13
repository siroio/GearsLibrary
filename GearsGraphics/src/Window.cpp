#include <Window.h>
#include <algorithm>
#include <ranges>
#include <deque>
#include <Internal/DX12/DirectX12.h>
#include <Vector2.h>
#include <StringUtility.h>

namespace
{
    WNDCLASSEX s_windowClass;
    HINSTANCE s_hInstance;
    HWND s_windowHandle;
    std::deque<Glib::IWindowMessage*> s_windowProcedures;
    std::string s_windowName{ "GameWindow" };
    Vector2 s_windowSize{ 1240.0f, 720.0f };
    Vector2 s_windowDebugSize{ 1240.0f, 720.0f };
    bool s_isBorderless{ false };
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    for (auto* proc : s_windowProcedures)
    {
        if (proc == nullptr) continue;
        (*proc)(hwnd, msg, wparam, lparam);
    }

    switch (msg)
    {
        case WM_DESTROY:
            s_windowProcedures.clear();
            CoUninitialize();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}

bool Glib::Window::Initialize()
{
    // 作成済みかチェック
    if (s_windowHandle != NULL) return false;

    // Comの初期化
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) return false;

    // ウィンドウの設定
    s_windowClass.cbSize = sizeof(WNDCLASSEX);
    GetModuleHandleEx(0, nullptr, &s_hInstance);
    s_windowClass.hInstance = s_hInstance;
    s_windowClass.lpfnWndProc = (WNDPROC)::WindowProcedure;


#if defined(DEBUG) || defined(_DEBUG)
    RECT rect{ 0, 0, static_cast<LONG>(s_windowDebugSize.x), static_cast<LONG>(s_windowDebugSize.y) };
    auto style = WS_OVERLAPPEDWINDOW;
#else
    RECT rect{ 0, 0, static_cast<LONG>(s_windowSize.x), static_cast<LONG>(s_windowSize.y) };
    auto style = s_isBorderless ? WS_POPUP | WS_BORDER : WS_OVERLAPPEDWINDOW;
#endif

#ifdef UNICODE
    s_windowClass.lpszClassName = StringToWide(s_windowName).c_str();
#else
    s_windowClass.lpszClassName = s_windowName.c_str();
#endif

    if (!RegisterClassEx(&s_windowClass)) return false;
    AdjustWindowRect(&rect, style, false);

    // ウィンドウ作成
    s_windowHandle = CreateWindowEx(
        0,
        s_windowClass.lpszClassName,
        s_windowClass.lpszClassName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        s_hInstance,
        nullptr
    );

    if (s_windowHandle == NULL) return false;

    ShowWindow(s_windowHandle, SW_SHOW);
    UpdateWindow(s_windowHandle);
    SetFocus(s_windowHandle);

    return true;
}

void Glib::Window::Finalize()
{
    UnregisterClass(s_windowClass.lpszClassName, s_hInstance);
    s_hInstance = nullptr;
    s_windowHandle = nullptr;
}

HINSTANCE Glib::Window::InstanceHandle()
{
    return s_hInstance;
}

HWND Glib::Window::WindowHandle()
{
    return s_windowHandle;
}

void Glib::Window::RegisterProcedure(IWindowMessage* const proc)
{
    if (proc == nullptr) return;
    s_windowProcedures.push_back(proc);
}

void Glib::Window::UnRegisterProcedure(IWindowMessage* const proc)
{
    if (s_windowProcedures.empty()) return;
    std::erase_if(s_windowProcedures, [&](IWindowMessage* value)
    {
        return value == proc;
    });
}

std::string& Glib::Window::WindowName()
{
    return s_windowName;
}

void Glib::Window::WindowName(const std::string& name)
{
    if (s_windowHandle != NULL) return;
    s_windowName = name;
}

const Vector2& Glib::Window::WindowSize()
{
    return s_windowSize;
}

void Glib::Window::WindowSize(const Vector2& size)
{
    if (s_windowHandle != NULL) return;
    s_windowSize = size;
}

const Vector2& Glib::Window::WindowDebugSize()
{
    return s_windowDebugSize;
}

void Glib::Window::WindowDebugSize(const Vector2& size)
{
    if (s_windowHandle != NULL) return;
    s_windowDebugSize = size;
}

bool Glib::Window::BorderlessWindow()
{
    return s_isBorderless;
}

void Glib::Window::BorderlessWindow(bool borderless)
{
    if (s_windowHandle != NULL) return;
    s_isBorderless = borderless;
}
