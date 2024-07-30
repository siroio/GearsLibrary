#include <InputSystem.h>
#include <Internal/GamePadDevice.h>
#include <Internal/KeyBoardDevice.h>
#include <Internal/MouseDevice.h>
#include <InputState.h>
#include <Vector2.h>
#include <Debugger.h>
#include <unordered_map>
#include <deque>

using namespace Glib::Internal;

namespace
{
    std::unique_ptr<Input::GamePadDevice> s_gamePad;
    std::unique_ptr<Input::KeyBoardDevice> s_keyBoard;
    std::unique_ptr<Input::MouseDevice> s_mouse;
    std::unordered_map<std::string, std::deque<Glib::InputState>> s_inputState;
}

bool Glib::InputSystem::Initialize()
{
    s_gamePad = std::make_unique<Input::GamePadDevice>();
    s_keyBoard = std::make_unique<Input::KeyBoardDevice>();
    s_mouse = std::make_unique<Input::MouseDevice>();

    // 生成できたか確認
    if (!(s_gamePad && s_keyBoard && s_mouse)) return false;

    // 初期化を確認
    if (!s_gamePad->Initialize()) return false;
    if (!s_keyBoard->Initialize()) return false;
    if (!s_mouse->Initialize()) return false;

    return true;
}

void Glib::InputSystem::Finalize()
{
    s_gamePad.reset();
    s_keyBoard.reset();
    s_mouse.reset();
    s_inputState.clear();
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
        Debug::Warn("Unknown Input: " + name);
        return false;
    }
    for (const auto& key : state->second)
    {
        switch (key.input.index())
        {
            case InputType::MOUSE:
                return GetMousePressed(std::get<InputType::MOUSE>(key.input));
            case InputType::KEYBOARD:
                return GetKey(std::get<InputType::KEYBOARD>(key.input));
            case InputType::GAMEPAD:
                return GetButton(std::get<InputType::GAMEPAD>(key.input));
            default:
                break;
        }
    }
    return false;
}

bool Glib::InputSystem::GetInputDown(const std::string& name)
{
    auto state = s_inputState.find(name);
    if (state == s_inputState.end())
    {
        Debug::Warn("Unknown Input: " + name);
        return false;
    }
    for (const auto& key : state->second)
    {
        switch (key.input.index())
        {
            case InputType::MOUSE:
                return GetMouseDown(std::get<InputType::MOUSE>(key.input));
            case InputType::KEYBOARD:
                return GetKeyDown(std::get<InputType::KEYBOARD>(key.input));
            case InputType::GAMEPAD:
                return GetButtonDown(std::get<InputType::GAMEPAD>(key.input));
            default:
                break;
        }
    }
    return false;
}

bool Glib::InputSystem::GetInputUp(const std::string& name)
{
    auto state = s_inputState.find(name);
    if (state == s_inputState.end())
    {
        Debug::Warn("Unknown Input: " + name);
        return false;
    }
    for (const auto& key : state->second)
    {
        switch (key.input.index())
        {
            case InputType::MOUSE:
                return GetMouseUp(std::get<InputType::MOUSE>(key.input));
            case InputType::KEYBOARD:
                return GetKeyUp(std::get<InputType::KEYBOARD>(key.input));
            case InputType::GAMEPAD:
                return GetButtonUp(std::get<InputType::GAMEPAD>(key.input));
            default:
                break;
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
    return s_gamePad->GetButton(static_cast<Input::PadNum>(pad), button);
}

bool Glib::InputSystem::GetButtonDown(GPADKey button, unsigned int pad)
{
    return s_gamePad->GetButtonDown(static_cast<Input::PadNum>(pad), button);
}

bool Glib::InputSystem::GetButtonUp(GPADKey button, unsigned int pad)
{
    return s_gamePad->GetButtonUp(static_cast<Input::PadNum>(pad), button);
}

Vector2 Glib::InputSystem::GetLeftStick(unsigned int pad, float deadZone)
{
    return s_gamePad->GetLeftStick(static_cast<Input::PadNum>(pad), deadZone);
}

Vector2 Glib::InputSystem::GetRightStick(unsigned int pad, float deadZone)
{
    return s_gamePad->GetRightStick(static_cast<Input::PadNum>(pad), deadZone);
}

float Glib::InputSystem::GetLeftTrigger(unsigned int pad, float deadZone)
{
    return s_gamePad->GetLeftTrigger(static_cast<Input::PadNum>(pad), deadZone);
}

float Glib::InputSystem::GetRightTrigger(unsigned int pad, float deadZone)
{
    return s_gamePad->GetRightTrigger(static_cast<Input::PadNum>(pad), deadZone);
}

bool Glib::InputSystem::GetMouseDown(MouseButton button)
{
    return s_mouse->ButtonDown(button);
}

bool Glib::InputSystem::GetMouseUp(MouseButton button)
{
    return s_mouse->ButtonUP(button);
}

bool Glib::InputSystem::GetMousePressed(MouseButton button)
{
    return s_mouse->Pressed(button);
}

Vector2 Glib::InputSystem::GetMousePosition()
{
    return s_mouse->Position();
}

Vector2 Glib::InputSystem::GetMouseDelta()
{
    return s_mouse->Delta();
}

float Glib::InputSystem::GetMouseWheel()
{
    return s_mouse->Wheel();
}

void Glib::InputSystem::ShowCursor()
{
    s_mouse->ShowCursor();
}

void Glib::InputSystem::HideCursor()
{
    s_mouse->HideCursor();
}

void Glib::InputSystem::SetPosition(const Vector2& position)
{
    s_mouse->SetPosition(position);
}
