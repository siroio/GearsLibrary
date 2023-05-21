#ifndef GEARS_GAME_OBJECT_H
#define GEARS_GAME_OBJECT_H

#include <concepts>
#include <string_view>
#include <memory>
#include <deque>
#include <Internal/IGameObject.h>
#include <GameObjectManager.h>

class Component;
class Transform;

template<class T>
concept IsComponent = std::derived_from<T, Component>;

class GameObject final :
    public GLib::Internal::Interface::IGameObject,
    public std::enable_shared_from_this<GameObject>
{
private:
    bool isDead_{ false };
    bool isActive_{ false };
    bool isDontDestroyOnLoad_{ false };
    std::string name_{ "" };
    std::string tag_{ "" };
    std::deque<std::shared_ptr<Component>> components_;
    GLib::Utility::WeakPtr<Transform> transform_{ nullptr };

public:
    explicit GameObject(std::string_view name);

    ~GameObject();

    bool IsAcive() const;

    void Active(bool active);

    template<class T, class... Args> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> AddComponent(Args&&... args);

    template<class T> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> GetComponent() const;

    template<class T> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> GetComponentInChildren() const;

    template<class T> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> GetComponentInParent() const;

    template<class T> requires IsComponent<T>
    std::deque<GLib::Utility::WeakPtr<T>> GetComponents() const;

    template<class T> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> GetComponentsInChildren() const;

    template<class T> requires IsComponent<T>
    GLib::Utility::WeakPtr<T> GetComponentsInParent() const;

    void RemoveComponents();

    void RemoveDeadComponents();

    void Destroy();

    bool DontDestroyOnLoad() const;

    void DontDestroyOnLoad(bool dontDestroyOnLoad);

    void Name(std::string_view name);

    std::string_view Name() const;

    void Tag(std::string_view tag);

    std::string_view Tag() const;

    bool IsDead() const;

    const GLib::Utility::WeakPtr<Transform>& Transform() const;

private:
    virtual void Initialize() override;
};

template<class T, class... Args> requires IsComponent<T>
inline GLib::Utility::WeakPtr<T> GameObject::AddComponent(Args&& ...args)
{
    return GLib::Utility::WeakPtr<T>();
}

template<class T> requires IsComponent<T>
inline GLib::Utility::WeakPtr<T> GameObject::GetComponent() const
{
    return GLib::Utility::WeakPtr<T>();
}

template<class T> requires IsComponent<T>
inline GLib::Utility::WeakPtr<T> GameObject::GetComponentInChildren() const
{
    return GLib::Utility::WeakPtr<T>();
}

template<class T> requires IsComponent<T>
inline GLib::Utility::WeakPtr<T> GameObject::GetComponentInParent() const
{
    return GLib::Utility::WeakPtr<T>();
}

template<class T> requires IsComponent<T>
inline std::deque<GLib::Utility::WeakPtr<T>> GameObject::GetComponents() const
{
    return std::list<GLib::Utility::WeakPtr<T>>();
}

template<class T> requires IsComponent<T>
inline GLib::Utility::WeakPtr<T> GameObject::GetComponentsInChildren() const
{
    return GLib::Utility::WeakPtr<T>();
}

template<class T> requires IsComponent<T>
inline GLib::Utility::WeakPtr<T> GameObject::GetComponentsInParent() const
{
    return GLib::Utility::WeakPtr<T>();
}

#endif // !GEARS_GAME_OBJECT_H
