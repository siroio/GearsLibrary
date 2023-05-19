#include <GameObjectManager.h>
#include <Internal/IGameObject.h>
#include <GameObject.h>
#include <WeakPtr.h>

void GameObjectManager::Update()
{
    gameObjects_.remove_if([](const std::shared_ptr<GameObject>& gameObject)
    {
        return false;
    });
}

void GameObjectManager::DebugDraw()
{}

void GameObjectManager::Finalize()
{
    gameObjects_.clear();
}

void GameObjectManager::ResetActorList()
{}

GameObjectPtr GameObjectManager::CreateActor()
{
    return CreateActor("Game Object");
}

GameObjectPtr GameObjectManager::CreateActor(std::string_view name)
{
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(name);
    gameObjects_.push_back(gameObject);
    GLib::Utility::WeakPtr<GLib::Internal::Interface::IGameObject> iGameObj{ gameObject };
    iGameObj->Initialize();
    return GameObjectPtr{ gameObject };
}

GameObjectPtr GameObjectManager::Find(std::string_view name)
{
    for (auto&& go : gameObjects_)
    {
        return GameObjectPtr();
    }
}

std::list<GameObjectPtr> GameObjectManager::FindGameObjectsWithTag(std::string_view tag)
{
    return std::list<GameObjectPtr>();
}

GameObjectPtr GameObjectManager::FindGameObjectWithTag(std::string_view tag)
{
    return GameObjectPtr();
}
