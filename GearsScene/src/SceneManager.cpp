#include <SceneManager.h>
#include <Scene.h>
#include <GameObjectManager.h>

namespace
{
    auto gameObjectManager = GameObjectManager::Instance();
}

void GLib::Scene::SceneManager::Update()
{
    if (next_.expired()) return;
    if (!current_.expired()) current_->End();
    gameObjectManager->ResetActorList();
    current_ = next_;
    next_ = nullptr;
    current_->Start();
}

void GLib::Scene::SceneManager::LoadScene(const std::string& name)
{
    if (scenes_.find(name) == scenes_.end())
    {
        //TODO: ERROR HANDLE
        return;
    }
    next_ = scenes_[name];
}
