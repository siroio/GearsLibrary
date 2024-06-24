#include <SceneManager.h>
#include <Scene.h>
#include <GameObjectManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Debugger.h>

namespace
{
    auto s_gameObjectManager = Glib::GameObjectManager::Instance();
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

void Glib::SceneManager::Update()
{
    if (next_.expired()) return;
    if (!current_.expired()) current_->End();
    s_dx12->WaitGPUPrevFrame();
    s_gameObjectManager->ResetGameObjects();
    s_gameObjectManager->RemoveDestroyableObjects();
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
