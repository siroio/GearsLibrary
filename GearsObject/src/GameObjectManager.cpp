#include <GameObjectManager.h>
#include <Internal/IGameObject.h>
#include <GameObject.h>
#include <WeakPtr.h>
#include <algorithm>
#include <iterator>

void Glib::GameObjectManager::Update()
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

void Glib::GameObjectManager::DebugDraw()
{}

void Glib::GameObjectManager::Finalize()
{
    gameObjects_.clear();
}

void Glib::GameObjectManager::ResetGameObjects()
{
    for (const auto& gameObject : gameObjects_)
    {
        if (gameObject->DontDestroyOnLoad()) continue;
        gameObject->Destroy();
    }
}

GameObjectPtr Glib::GameObjectManager::Instantiate()
{
    return gameObjects_.empty() ?
        Instantiate("GameObject") :
        Instantiate("GameObject " + std::to_string(gameObjects_.size()));
}

GameObjectPtr Glib::GameObjectManager::Instantiate(std::string_view name)
{
    const auto gameObject = std::make_shared<GameObject>(name);
    gameObjects_.push_back(gameObject);
    const WeakPtr<Internal::Interface::IGameObject> iGameObj{ gameObject };
    iGameObj->Initialize();
    return GameObjectPtr{ gameObject };
}

GameObjectPtr Glib::GameObjectManager::Find(std::string_view name)
{
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [name](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Name().compare(name);
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}

std::list<GameObjectPtr> Glib::GameObjectManager::FindGameObjectsWithTag(std::string_view tag)
{
    std::list<GameObjectPtr> result;

    std::ranges::copy_if(gameObjects_.begin(), gameObjects_.end(), std::back_inserter(result), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag().compare(tag);
    });

    return result;
}

GameObjectPtr Glib::GameObjectManager::FindGameObjectWithTag(std::string_view tag)
{
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag().compare(tag);
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}
