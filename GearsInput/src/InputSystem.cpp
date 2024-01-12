#include <InputSystem.h>
#include <unordered_map>
#include <deque>
#include <Internal/GamePadDevice.h>
#include <Internal/KeyBoardDevice.h>
#include <Internal/MouseDevice.h>
#include <Vector2.h>
#include <InputState.h>
#include <Debugger.h>

namespace
{
    ComPtr<IDirectInput8> s_dinput;
    std::unique_ptr<Glib::Internal::Input::GamePadDevice> s_gamePad;
    std::unique_ptr<Glib::Internal::Input::KeyBoardDevice> s_keyBoard;
    std::unique_ptr<Glib::Internal::Input::MouseDevice> s_mouse;
    std::unordered_map<std::string, std::deque<Glib::InputState>> s_inputState;
}

bool Glib::InputSystem::Initialize()
{
    if (s_dinput != nullptr) return false;

    // LPDIRECTINPUT8を作成
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
    s_mouse = std::make_unique<Glib::Internal::Input::MouseDevice>();

    s_gamePad->Initialize(s_dinput);
    s_keyBoard->Initialize(s_dinput);
    s_mouse->Initialize(s_dinput);

    return true;
}

void Glib::InputSystem::Update()
{
    s_gamePad->Update();
    s_keyBoard->Update();
    s_mouse->Update();
}

void Glib::InputSystem::AddInput(const std::string& name, MouseButton button)
{
    s_inputState[name].emplace_back(button);
}

void Glib::InputSystem::AddInput(const std::string& name, KeyCode button)
{
    s_inputState[name].emplace_back(button);
}

void Glib::InputSystem::AddInput(const std::string& name, GPADKey button)
{
    s_inputState[name].emplace_back(button);
}

bool Glib::InputSystem::GetInput(const std::string& name)
{
    auto state = s_inputState.find(name);
    if (state == s_inputState.end())
    {
        Debug::Log("Unknown Input: " + name);
        return false;
    }
    for (const auto& key : state->second)
    {
        switch (key.type)
        {
            case InputType::MOUSE:
                return false;
            case InputType::KEYBOARD:
                return GetKey(key.keyboard);
            case InputType::GAMEPAD:
                return GetButton(key.pad);
        }
    }
    return false;
}

bool Glib::InputSystem::GetInputDown(const std::string& name)
{
    auto state = s_inputState.find(name);
    if (state == s_inputState.end())
    {
        Debug::Log("Unknown Input: " + name);
        return false;
    }
    for (const auto& key : state->second)
    {
        switch (key.type)
        {
            case InputType::MOUSE:
                return false;
            case InputType::KEYBOARD:
                return GetKeyDown(key.keyboard);
            case InputType::GAMEPAD:
                return GetButtonDown(key.pad);
        }
    }
    return false;
}

bool Glib::InputSystem::GetInputUp(const std::string& name)
{
    auto state = s_inputState.find(name);
    if (state == s_inputState.end())
    {
        Debug::Log("Unknown Input: " + name);
        return false;
    }
    for (const auto& key : state->second)
    {
        switch (key.type)
        {
            case InputType::MOUSE:
                return false;
            case InputType::KEYBOARD:
                return GetKeyUp(key.keyboard);
            case InputType::GAMEPAD:
                return GetButtonUp(key.pad);
        }
    }
    return false;
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

bool Glib::InputSystem::GetButton(GPADKey button, unsigned int pad)
{
    return s_gamePad->GetButton(static_cast<Glib::Internal::Input::PadNum>(pad), button);
}

bool Glib::InputSystem::GetButtonDown(GPADKey button, unsigned int pad)
{
    return s_gamePad->GetButtonDown(static_cast<Glib::Internal::Input::PadNum>(pad), button);
}

bool Glib::InputSystem::GetButtonUp(GPADKey button, unsigned int pad)
{
    return s_gamePad->GetButtonUp(static_cast<Glib::Internal::Input::PadNum>(pad), button);
}

Vector2 Glib::InputSystem::GetLeftStick(unsigned int pad, float deadZone)
{
    return s_gamePad->GetLeftStick(static_cast<Glib::Internal::Input::PadNum>(pad), deadZone);
}

Vector2 Glib::InputSystem::GetRightStick(unsigned int pad, float deadZone)
{
    return s_gamePad->GetRightStick(static_cast<Glib::Internal::Input::PadNum>(pad), deadZone);
}

float Glib::InputSystem::GetLeftTrigger(unsigned int pad, float deadZone)
{
    return s_gamePad->GetLeftTrigger(static_cast<Glib::Internal::Input::PadNum>(pad), deadZone);
}

float Glib::InputSystem::GetRightTrigger(unsigned int pad, float deadZone)
{
    return s_gamePad->GetRightTrigger(static_cast<Glib::Internal::Input::PadNum>(pad), deadZone);
}
