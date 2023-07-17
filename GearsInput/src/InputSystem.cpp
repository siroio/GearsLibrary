#include <InputSystem.h>
#include <Internal/KeyBoardDevice.h>
#include <Internal/GamePadDevice.h>
#include <Logger.h>

namespace
{
    ComPtr<IDirectInput8> dinput_;
    std::unique_ptr<Glib::Internal::Input::GamePadDevice> gamePad_;
    std::unique_ptr<Glib::Internal::Input::KeyBoardDevice> keyBoard_;
}

bool Glib::InputSystem::Initialize()
{
    if (dinput_ != nullptr) return false;

    // LPDIRECTINPUT8ÇçÏê¨
    auto res = FAILED(DirectInput8Create(
        GetModuleHandle(nullptr),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)dinput_.ReleaseAndGetAddressOf(),
        nullptr
    ));
    if (res) return false;

    gamePad_ = std::make_unique<Glib::Internal::Input::GamePadDevice>();
    keyBoard_ = std::make_unique<Glib::Internal::Input::KeyBoardDevice>();

    bool initialized = keyBoard_->Initialize(dinput_);
    if (!initialized) return false;

    initialized = gamePad_->Initialize(dinput_);
    if (!initialized) return false;
    return true;
}

void Glib::InputSystem::Update()
{
    keyBoard_->Update();
}

bool Glib::InputSystem::GetKey(KeyCode key)
{
    return keyBoard_->GetKey(key);
}

bool Glib::InputSystem::GetKeyDown(KeyCode key)
{
    return keyBoard_->GetKeyDown(key);
}

bool Glib::InputSystem::GetKeyUp(KeyCode key)
{
    return keyBoard_->GetKeyUp(key);
}

Glib::InputSystem::~InputSystem()
{

}
