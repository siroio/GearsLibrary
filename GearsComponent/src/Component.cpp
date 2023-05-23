#include <Component.h>

bool Component::Active() const
{
    return false;
}

void Component::Active(bool active)
{}

const GameObjectPtr& Component::GameObject() const
{}

void Component::Destroy()
{}

bool Component::IsDead() const
{
    return false;
}

void Component::SetGameObject(const GameObjectPtr& gameObject)
{}
