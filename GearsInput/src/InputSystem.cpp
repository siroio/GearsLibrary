#include <InputSystem.h>
#include <Internal/GamePadDevice.h>
#include <Internal/KeyBoardDevice.h>
#include <Logger.h>

namespace
{
    ComPtr<IDirectInput8> s_dinput;
    std::unique_ptr<Glib::Internal::Input::GamePadDevice> s_gamePad;
    std::unique_ptr<Glib::Internal::Input::KeyBoardDevice> s_keyBoard;
}

bool Glib::InputSystem::Initialize()
{
    if (s_dinput != nullptr) return false;

    // LPDIRECTINPUT8ÇçÏê¨
    auto res = FAILED(DirectInput8Create(
        GetModuleHandle(nullptr),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<void**>(s_dinput.ReleaseAndGetAddressOf()),
        nullptr
    ));
    if (res) return false;

    s_gamePad = std::make_unique<Glib::Internal::Input::GamePadDevice>();
    s_keyBoard = std::make_unique<Glib::Internal::Input::KeyBoardDevice>();

    bool initialized = s_gamePad->Initialize(s_dinput);
    initialized = s_keyBoard->Initialize(s_dinput);

    return true;
}

void Glib::InputSystem::Update()
{
    s_gamePad->Update();
    s_keyBoard->Update();
}

bool Glib::InputSystem::GetKey(KeyCode key)
{
    return s_keyBoard->GetKey(key);
}

bool Glib::InputSystem::GetKeyDown(KeyCode key)
{
    return s_keyBoard->GetKeyDown(key);
}

bool Glib::InputSystem::GetKeyUp(KeyCode key)
{
    return s_keyBoard->GetKeyUp(key);
}
