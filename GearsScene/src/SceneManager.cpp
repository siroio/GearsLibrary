#include <SceneManager.h>
#include <Scene.h>
#include <GameObjectManager.h>
#include <Logger.h>

namespace
{
    auto gameObjectManager = GameObjectManager::Instance();
}

void Glib::Scene::SceneManager::Update()
{
    if (next_.expired()) return;
    if (!current_.expired()) current_->End();
    gameObjectManager->ResetGameObjects();
    current_ = next_;
    next_ = nullptr;
    current_->Start();
}

void Glib::Scene::SceneManager::LoadScene(const std::string& name)
{
    if (scenes_.find(name) == scenes_.end())
    {
        Debug::Log("Scene " + name + " was not found.");
        return;
    }
    next_ = scenes_[name];
}
