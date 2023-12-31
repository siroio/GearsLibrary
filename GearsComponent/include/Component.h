#pragma once
#include <Internal/IComponent.h>
#include <GameObjectPtr.h>

/**
 * @brief コンポーネント基底クラス
 */
class Component :
    public Glib::Internal::Interface::IComponent,
    public std::enable_shared_from_this<Component>
{
protected:
    Component() = default;
    virtual ~Component() = default;

public:
    bool Active() const;
    void Active(bool active);
    const GameObjectPtr& GameObject() const;
    void Destroy();
    bool IsDead() const;
    virtual void OnGUI();

private:
    virtual void SetGameObject(const GameObjectPtr& gameObject) override;

private:
    bool isActive_{ true };
    bool isDead_{ false };
    GameObjectPtr gameObject_{ nullptr };
};
