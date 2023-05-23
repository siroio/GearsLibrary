#ifndef GEARS_COMPONENT_H
#define GEARS_COMPONENT_H

#include <Internal/IComponent.h>
#include <GameObjectPtr.h>

class Component :
    public GLib::Internal::Interface::IComponent,
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

private:
    virtual void SetGameObject(const GameObjectPtr& gameObject) override;

private:
    bool isActive_{ true };
    bool isDead_{ false };
    GameObjectPtr gameObject_{ nullptr };
};

#endif // !GEARS_COMPONENT_H
