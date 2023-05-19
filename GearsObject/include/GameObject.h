#ifndef GEARS_GAME_OBJECT_H
#define GEARS_GAME_OBJECT_H

#include <memory>
#include <string_view>
#include <concepts>
#include <Internal/IGameObject.h>
#include <GameObjectManager.h>

class Component;

template<class T>
concept IsComponent = std::derived_from<T, Component>;

class GameObject final :
    public GLib::Internal::Interface::IGameObject,
    public std::enable_shared_from_this<GameObject>
{
public:
    explicit GameObject(std::string_view name);
    virtual void Initialize() override;

    bool IsAcive() const;
    void Active(bool enable);

    template<class T, class... Args>
    GLib::Utility::WeakPtr<T> AddComponent(Args&&... args);

    template<class T> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> GetComponent() const;

    void Destroy();
    void DontDestroyOnLoad();

};

#endif // !GEARS_GAME_OBJECT_H
