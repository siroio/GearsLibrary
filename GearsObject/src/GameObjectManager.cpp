#include <GameObjectManager.h>
#include <Internal/IGameObject.h>
#include <GameObject.h>
#include <WeakPtr.h>
#include <algorithm>
#include <iterator>

void GameObjectManager::Update()
{
    std::erase_if(gameObjects_, [](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->IsDead();
    });

    for (const auto& gameObject : gameObjects_)
    {
        gameObject->RemoveDeadComponents();
    }
}

void GameObjectManager::DebugDraw()
{}

void GameObjectManager::Finalize()
{
    gameObjects_.clear();
}

void GameObjectManager::ResetActorList()
{
    for (const auto& gameObject : gameObjects_)
    {
        if (gameObject->DontDestroyOnLoad()) continue;
        gameObject->Destroy();
    }
}

GameObjectPtr GameObjectManager::CreateActor()
{
    return CreateActor("GameObject");
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
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [name](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Name() == name;
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}

std::deque<GameObjectPtr> GameObjectManager::FindGameObjectsWithTag(std::string_view tag)
{
    std::deque<GameObjectPtr> result;

    std::ranges::copy_if(gameObjects_.begin(), gameObjects_.end(), std::back_inserter(result), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag() == tag;
    });

    return result;
}

GameObjectPtr GameObjectManager::FindGameObjectWithTag(std::string_view tag)
{
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag() == tag;
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}
