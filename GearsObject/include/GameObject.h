#pragma once
#include <concepts>
#include <string_view>
#include <algorithm>
#include <memory>
#include <deque>
#include <list>
#include <Internal/IGameObject.h>
#include <Internal/ComponentManager.h>
#include <GameObjectManager.h>
#include <Components/Transform.h>
#include <EventMsg.h>

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
     * @brief ゲームオブジェクトのすべてのコンポーネントをすべて取得
     * @return コンポーネント一覧
     */
    template<class T> requires IsComponent<T>
    std::list<Glib::WeakPtr<T>> GetComponents() const;

    /**
     * @brief 親子関係にある子オブジェクトからコンポーネントをすべて取得
     * @return コンポーネント一覧
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentsInChildren() const;

    /**
     * @brief 親子関係にある親オブジェクトからコンポーネントをすべて取得
     * @return コンポーネント一覧
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentsInParent() const;

    /**
     * @brief メッセージを送信
     */
    template<class T>
    void SendMsg(unsigned int msgID, const T& value, const GameObjectPtr& other);

    /**
     * @brief メッセージを自身へ送信
     */
    template<class T>
    void SendMsg(unsigned int msgID, const T& value);

    /**
     * @brief コンポーネントの削除
     */
    void RemoveComponents();

    /**
     * @brief 死亡したコンポーネントの削除
     */
    void RemoveDeadComponents();

    /**
     * @brief デバッグ用GUI描画
     */
    void DrawGUI();

    /**
     * @brief ゲームオブジェクトの削除
     */
    void Destroy();

    /**
     * @brief ゲームオブジェクトが削除されないか
     */
    bool DontDestroyOnLoad() const;

    /**
     * @brief ゲームオブジェクトのDestroyを防ぐ
     * @param dontDestroyOnLoad
     */
    void DontDestroyOnLoad(bool dontDestroyOnLoad);

    /**
     * @brief ゲームオブジェクトがアクティブか
     * @return bool
     */
    bool Active() const;

    /**
     * @brief ゲームオブジェクトのアクティブを設定
     * @param active
     */
    void Active(bool active);

    /**
     * @brief ゲームオブジェクトの名前を設定
     * @param name
     */
    void Name(std::string_view name);

    /**
     * @brief ゲームオブジェクトの名前を変更
     * @return name
     */
    std::string Name() const;

    /**
     * @brief ゲームオブジェクトのタグを設定
     * @param tag
     */
    void Tag(std::string_view tag);

    /**
     * @brief ゲームオブジェクトのタグを取得
     * @return tag
     */
    std::string Tag() const;

    /**
     * @brief レイヤー
     */
    unsigned int Layer() const;

    /**
     * @brief レイヤーの設定
     * @param layer
     */
    void Layer(unsigned int layer);

    /**
     * @brief ゲームオブジェクトが死亡状態か
     * @return bool
     */
    bool IsDead() const;

    /**
     * @brief ルートオブジェクトか
     */
    bool IsRoot() const;

    /**
     * @brief ゲームオブジェクトのトランスフォームを取得
     * @return
     */
    const Glib::WeakPtr<Glib::Transform>& Transform() const;

private:
    virtual void Initialize() override;
    void ReceiveMsg(const Glib::EventMsg& msg);

private:
    bool isDead_{ false };
    bool isActive_{ true };
    bool isDontDestroyOnLoad_{ false };
    unsigned int layer_{ 0 };
    std::string name_{ "" };
    std::string tag_{ "" };
    std::deque<std::shared_ptr<Component>> components_;
    Glib::WeakPtr<Glib::Transform> transform_{ nullptr };
};

template<class T, class... Args> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::AddComponent(Args&& ...args)
{
    auto manager = Glib::Internal::ComponentManager::Instance();
    auto component = manager->AddComponent<T>(weak_from_this(), std::forward<Args>(args)...);
    components_.push_back(component);
    return Glib::WeakPtr<T>{ component };
}

template<class T> requires IsComponent<T>
inline Glib::WeakPtr<T> GameObject::GetComponent() const
{
    for (const auto& component : components_)
    {
        if (typeid(T) != std::remove_pointer<decltype(component.get())>::type) continue;
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

    Glib::WeakPtr<Glib::Transform> parent = transform_->Parent();
    if (!parent.expired()) return parent->GameObject()->GetComponentInParent<T>();

    return Glib::WeakPtr<T>{ nullptr };
}

template<class T> requires IsComponent<T>
inline std::list<Glib::WeakPtr<T>> GameObject::GetComponents() const
{
    std::list<Glib::WeakPtr<T>> result;

    for (const auto& component : components_)
    {
        if (typeid(T) != std::remove_pointer<decltype(component.get())>::type) continue;
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
    Glib::WeakPtr<Glib::Transform> parent = transform_->Parent();
    if (!parent.expired())
    {
        std::list<Glib::WeakPtr<T>> parentComponents = parent->GameObject()->GetComponentsInParent();
        components.splice(components.end(), std::move(parentComponents));
    }

    return components;
}

template<class T>
inline void GameObject::SendMsg(unsigned int msgID, const T& value, const GameObjectPtr& other)
{
    other->ReceiveMsg(Glib::EventMsg{ msgID, value });
}

template<class T>
inline void GameObject::SendMsg(unsigned int msgID, const T& value)
{
    shared_from_this()->SendMsg(msgID, value, shared_from_this());
}
