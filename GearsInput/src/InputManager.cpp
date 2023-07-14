#include <InputManager.h>
#include <Internal/MouseDevice.h>
#include <Logger.h>
namespace
{
    std::unique_ptr<Glib::Internal::Input::MouseDevice> mouse_;
}

bool Glib::InputManager::Initialize()
{
    mouse_ = std::make_unique<Glib::Internal::Input::MouseDevice>();
    return true;
}

void Glib::InputManager::Update()
{
    mouse_->Update();
}

Glib::InputManager::~InputManager()
{

}
