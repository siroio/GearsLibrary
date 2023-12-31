#include <SceneManager.h>
#include <Scene.h>
#include <GameObjectManager.h>
#include <Debugger.h>

namespace
{
    auto s_gameObjectManager = Glib::GameObjectManager::Instance();
}

void Glib::SceneManager::Update()
{
    if (next_.expired()) return;
    if (!current_.expired()) current_->End();
    s_gameObjectManager->ResetGameObjects();
    current_ = next_;
    next_ = nullptr;
    current_->Start();
}

void Glib::SceneManager::LoadScene(const std::string& name)
{
    if (!scenes_.contains(name))
    {
        Debug::Log("Scene " + name + " was not found.");
        return;
    }
    next_ = scenes_.at(name);
}

std::list<std::string> Glib::SceneManager::Scenes()
{
    std::list<std::string> scenes;
    for (const auto& [name, ptr] : scenes_)
    {
        scenes.push_back(name);
    }
    return scenes;
}
