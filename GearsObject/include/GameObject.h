#pragma once
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
 * @brief �Q�[���I�u�W�F�N�g�N���X
 */
class GameObject final :
    public Glib::Internal::Interface::IGameObject,
    public std::enable_shared_from_this<GameObject>
{
public:
    /**
     * @brief �Q�[���I�u�W�F�N�g�𐶐�
     * @param �I�u�W�F�N�g��
     */
    explicit GameObject(std::string_view name);

    ~GameObject();

    /**
     * @brief �R���|�[�l���g�̒ǉ�
     * @return �R���|�[�l���g
     */
    template<class T, class... Args> requires IsComponent<T>
    Glib::WeakPtr<T> AddComponent(Args&&... args);

    /**
     * @brief �R���|�[�l���g�̎擾
     * @return �R���|�[�l���g
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponent() const;

    /**
     * @brief �e�q�֌W�ɂ���q�I�u�W�F�N�g����R���|�[�l���g���擾
     * @return �R���|�[�l���g
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentInChildren() const;

    /**
     * @brief �e�q�֌W�ɂ���e�I�u�W�F�N�g����R���|�[�l���g���擾
     * @return �R���|�[�l���g
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentInParent() const;

    /**
     * @brief �Q�[���I�u�W�F�N�g�̂��ׂẴR���|�[�l���g�����ׂĎ擾
     * @return �R���|�[�l���g�ꗗ
     */
    template<class T> requires IsComponent<T>
    std::list<Glib::WeakPtr<T>> GetComponents() const;

    /**
     * @brief �e�q�֌W�ɂ���q�I�u�W�F�N�g����R���|�[�l���g�����ׂĎ擾
     * @return �R���|�[�l���g�ꗗ
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentsInChildren() const;

    /**
     * @brief �e�q�֌W�ɂ���e�I�u�W�F�N�g����R���|�[�l���g�����ׂĎ擾
     * @return �R���|�[�l���g�ꗗ
     */
    template<class T> requires IsComponent<T>
    Glib::WeakPtr<T> GetComponentsInParent() const;

    /**
     * @brief �R���|�[�l���g�̍폜
     */
    void RemoveComponents();

    /**
     * @brief ���S�����R���|�[�l���g�̍폜
     */
    void RemoveDeadComponents();

    /**
     * @brief �f�o�b�O�pGUI�`��
     */
    void DrawGUI();

    /**
     * @brief �Q�[���I�u�W�F�N�g�̍폜
     */
    void Destroy();

    /**
     * @brief �Q�[���I�u�W�F�N�g���폜����Ȃ���
     */
    bool DontDestroyOnLoad() const;

    /**
     * @brief �Q�[���I�u�W�F�N�g��Destroy��h��
     * @param dontDestroyOnLoad
     */
    void DontDestroyOnLoad(bool dontDestroyOnLoad);

    /**
     * @brief �Q�[���I�u�W�F�N�g���A�N�e�B�u��
     * @return bool
     */
    bool Active() const;

    /**
     * @brief �Q�[���I�u�W�F�N�g�̃A�N�e�B�u��ݒ�
     * @param active
     */
    void Active(bool active);

    /**
     * @brief �Q�[���I�u�W�F�N�g�̖��O��ݒ�
     * @param name
     */
    void Name(std::string_view name);

    /**
     * @brief �Q�[���I�u�W�F�N�g�̖��O��ύX
     * @return name
     */
    std::string Name() const;

    /**
     * @brief �Q�[���I�u�W�F�N�g�̃^�O��ݒ�
     * @param tag
     */
    void Tag(std::string_view tag);

    /**
     * @brief �Q�[���I�u�W�F�N�g�̃^�O���擾
     * @return tag
     */
    std::string Tag() const;

    /**
     * @brief ���C���[
     */
    unsigned int Layer() const;

    /**
     * @brief ���C���[�̐ݒ�
     * @param layer
     */
    void Layer(unsigned int layer);

    /**
     * @brief �Q�[���I�u�W�F�N�g�����S��Ԃ�
     * @return bool
     */
    bool IsDead() const;

    /**
     * @brief ���[�g�I�u�W�F�N�g��
     */
    bool IsRoot() const;

    /**
     * @brief �Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�����擾
     * @return
     */
    const Glib::WeakPtr<Glib::Transform>& Transform() const;

private:
    virtual void Initialize() override;

private:
    bool isDead_{ false };
    bool isActive_{ true };
    bool isDontDestroyOnLoad_{ false };
    unsigned int layer_{ 0 };
    std::string name_{ "" };
    std::string tag_{ "" };
    std::list<std::shared_ptr<Component>> components_;
    Glib::WeakPtr<Glib::Transform> transform_{ nullptr };
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
        if (typeid(T) != typeid(*component.get())) continue;
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
    Glib::WeakPtr<Glib::Transform> parent = transform_->Parent();
    if (!parent.expired())
    {
        std::list<Glib::WeakPtr<T>> parentComponents = parent->GameObject()->GetComponentsInParent();
        components.splice(components.end(), std::move(parentComponents));
    }

    return components;
}
