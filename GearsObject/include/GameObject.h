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
 * @brief ゲームオブジェクトクラス
 */
class GameObject final :
    public Glib::Internal::Interface::IGameObject,
    public std::enable_shared_from_this<GameObject>
{
public:
    /**
     * @brief ゲームオブジェクトを生成
     * @param オブジェクト名
     */
    explicit GameObject(std::string_view name);

    ~GameObject();

    /**
     * @brief コンポーネントの追加
     * @return コンポーネント
     */
    template<class T, class... Args> requires IsComponent<T>
    Glib::WeakPtr<T> AddComponent(Args&&... args);

    /**
     * @brief コンポーネントの取得
     * @return コンポーネント
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponent() const;

    /**
     * @brief 親子関係にある子オブジェクトからコンポーネントを取得
     * @return コンポーネント
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentInChildren() const;

    /**
     * @brief 親子関係にある親オブジェクトからコンポーネントを取得
     * @return コンポーネント
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentInParent() const;

    /**
     * @brief ゲームオブジェクトのすべてのコンポーネントを返します
     * @return コンポーネント一覧
     */
    template<class T> requires IsComponent<T>
    std::list<Glib::WeakPtr<T>> GetComponents() const;

    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentsInChildren() const;

    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentsInParent() const;

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

    const Glib::WeakPtr<Transform>& Transform() const;

private:
    virtual void Initialize() override;

private:
    bool isDead_{ false };
    bool isActive_{ true };
    bool isDontDestroyOnLoad_{ false };
    std::string name_{ "" };
    std::string tag_{ "" };
    std::list<std::shared_ptr<Component>> components_;
    Glib::WeakPtr<class Transform> transform_{ nullptr };
};

template<class T, class... Args> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::AddComponent(Args&& ...args)
{
    auto component = Glib::Internal::ComponentManager::Instance()->AddComponent<T>(weak_from_this(), args...);
    components_.push_back(component);
    return Glib::WeakPtr<T>{ component };
}

template<class T> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::GetComponent() const
{
    for (const auto& component : components_)
    {
        if (typeid(T) != typeid(*(component.get()))) continue;
        return Glib::WeakPtr<T>(std::dynamic_pointer_cast<T>(component));
    }

    return Glib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::GetComponentInChildren() const
{
    Glib::WeakPtr<T> component = GetComponent<T>();
    if (!component.expired()) return component;
    for (const auto& child : transform_->Children())
    {
        component = child->GameObject()->GetComponentInChildren<T>();
        if (!component.expired()) return component;
    }

    return Glib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::GetComponentInParent() const
{
    Glib::WeakPtr<T> component = GetComponent<T>();
    if (!component.expired()) return component;

    Glib::WeakPtr<class Transform> parent = transform_->Parent();
    if (!parent.expired()) return parent->GameObject()->GetComponentInParent<T>();

    return Glib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline std::list<Glib::WeakPtr<T>> GameObject::GetComponents() const
{
    std::list<Glib::WeakPtr<T>> result;

    for (const auto& component : components_)
    {
        if (typeid(T) != typeid(*(component.get()))) continue;
        result.push_back(Glib::WeakPtr<T>{ std::dynamic_pointer_cast<T>(component) });
    }

    return result;
}

template<class T> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::GetComponentsInChildren() const
{
    std::list<Glib::WeakPtr<T>> components = GetComponents<T>();

    for (const auto& child : transform_->Children())
    {
        std::list<Glib::WeakPtr<T>> childComponents = child->GameObject()->GetComponentsInChildren<T>();
        components.splice(components.end(), std::move(childComponents));
    }

    return components;
}

template<class T> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::GetComponentsInParent() const
{
    std::list<Glib::WeakPtr<T>> components = GetComponents<T>();
    Glib::WeakPtr<class Transform> parent = transform_->Parent();
    if (!parent.expired())
    {
        std::list<Glib::WeakPtr<T>> parentComponents = parent->GameObject()->GetComponentsInParent();
        components.splice(components.end(), std::move(parentComponents));
    }

    return components;
}

#endif // !GEARS_GAME_OBJECT_H
