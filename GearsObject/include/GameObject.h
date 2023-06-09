#ifndef GEARS_GAME_OBJECT_H
#define GEARS_GAME_OBJECT_H

#include <concepts>
#include <string_view>
#include <algorithm>
#include <memory>
#include <list>

#include <Internal/IGameObject.h>
#include <Internal/ComponentManager.h>
#include <GameObjectManager.h>

#include <Components/Transform.h>

class Component;

template<class T>
concept IsComponent = std::derived_from<T, Component>;

/**
 * @brief オブジェクトクラス
 */
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
    std::list<std::shared_ptr<Component>> components_;
    GLib::WeakPtr<Transform> transform_{ nullptr };

public:
    explicit GameObject(std::string_view name);

    ~GameObject();

    template<class T, class... Args> requires IsComponent<T>
    GLib::WeakPtr<T> AddComponent(Args&&... args);

    template<class T> requires IsComponent<T>
    GLib::WeakPtr<T> GetComponent() const;

    template<class T> requires IsComponent<T>
    GLib::WeakPtr<T> GetComponentInChildren() const;

    template<class T> requires IsComponent<T>
    GLib::WeakPtr<T> GetComponentInParent() const;

    template<class T> requires IsComponent<T>
    std::list<GLib::WeakPtr<T>> GetComponents() const;

    template<class T> requires IsComponent<T>
    GLib::WeakPtr<T> GetComponentsInChildren() const;

    template<class T> requires IsComponent<T>
    GLib::WeakPtr<T> GetComponentsInParent() const;

    void RemoveComponents();

    void RemoveDeadComponents();

    void Destroy();

    bool DontDestroyOnLoad() const;

    void DontDestroyOnLoad(bool dontDestroyOnLoad);

    bool Active() const;

    void Active(bool active);

    void Name(std::string_view name);

    std::string_view Name() const;

    void Tag(std::string_view tag);

    std::string_view Tag() const;

    bool IsDead() const;

    const GLib::WeakPtr<Transform>& Transform() const;

private:
    virtual void Initialize() override;
};

template<class T, class... Args> requires IsComponent<T>
inline GLib::WeakPtr<T> GameObject::AddComponent(Args&& ...args)
{
    auto component = GLib::Internal::ComponentManager::Instance()->AddComponent<T>(weak_from_this(), args...);
    components_.push_back(component);
    return GLib::WeakPtr<T>(component);
}

template<class T> requires IsComponent<T>
inline GLib::WeakPtr<T> GameObject::GetComponent() const
{
    for (const auto& component : components_)
    {
        if (typeid(T) != typeid(*(component.get()))) continue;
        return GLib::WeakPtr<T>(std::dynamic_pointer_cast<T>(component));
    }

    return GLib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline GLib::WeakPtr<T> GameObject::GetComponentInChildren() const
{
    GLib::WeakPtr<T> component = GetComponent<T>();
    if (!component.expired()) return component;
    for (const auto& child : transform_->Children())
    {
        component = child->GameObject()->GetComponentInChildren<T>();
        if (!component.expired()) return component;
    }

    return GLib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline GLib::WeakPtr<T> GameObject::GetComponentInParent() const
{
    GLib::WeakPtr<T> component = GetComponent<T>();
    if (!component.expired()) return component;

    GLib::WeakPtr<class Transform> parent = transform_->Parent();
    if (!parent.expired()) return parent->GameObject()->GetComponentInParent<T>();

    return GLib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline std::list<GLib::WeakPtr<T>> GameObject::GetComponents() const
{
    std::list<GLib::WeakPtr<T>> result;
    std::ranges::copy_if(components_.begin(), components_.end(), result, [](const std::shared_ptr<Component> component)
    {
        return typeid(T) == typeid(*(component.get()));
    });

    return result;
}

template<class T> requires IsComponent<T>
inline GLib::WeakPtr<T> GameObject::GetComponentsInChildren() const
{
    std::list<GLib::WeakPtr<T>> components = GetComponents<T>();

    for (const auto& child : transform_->Children())
    {
        std::list<GLib::WeakPtr<T>> childComponents = child->GameObject()->GetComponentsInChildren<T>();
        components.splice(components.end(), std::move(childComponents));
    }

    return components;
}

template<class T> requires IsComponent<T>
inline GLib::WeakPtr<T> GameObject::GetComponentsInParent() const
{
    std::list<GLib::WeakPtr<T>> components = GetComponents<T>();
    GLib::WeakPtr<class Transform> parent = transform_->Parent();
    if (!parent.expired())
    {
        std::list<GLib::WeakPtr<T>> parentComponents = parent->GameObject()->GetComponentsInParent();
        components.splice(components.end(), std::move(parentComponents));
    }

    return components;
}

#endif // !GEARS_GAME_OBJECT_H
