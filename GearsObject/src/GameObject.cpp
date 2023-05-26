#include <GameObject.h>
#include <Component.h>
#include <Components/Transform.h>

GameObject::GameObject(std::string_view name) : name_{ name }
{}

GameObject::~GameObject()
{
    RemoveComponents();
}

void GameObject::Initialize()
{}

void GameObject::RemoveComponents()
{
    components_.clear();
}

void GameObject::RemoveDeadComponents()
{
    std::erase_if(components_, [](const GLib::Utility::WeakPtr<Component>& component)
    {
        return component->IsDead();
    });
}

void GameObject::Destroy()
{
    isActive_ = false;
    isDead_ = true;

    for (const auto& component : components_)
    {
        component->Destroy();
    }
}

bool GameObject::DontDestroyOnLoad() const
{
    return isDontDestroyOnLoad_;
}

void GameObject::DontDestroyOnLoad(bool dontDestroyOnLoad)
{
    isDontDestroyOnLoad_ = dontDestroyOnLoad;
}

bool GameObject::Active() const
{
    return isActive_;
}

void GameObject::Active(bool active)
{
    isActive_ = active;
}


void GameObject::Name(std::string_view name)
{
    name_ = name;
}

std::string_view GameObject::Name() const
{
    return name_;
}

void GameObject::Tag(std::string_view tag)
{
    tag_ = tag;
}

std::string_view GameObject::Tag() const
{
    return tag_;
}

bool GameObject::IsDead() const
{
    return isDead_;
}

const GLib::Utility::WeakPtr<Transform>& GameObject::Transform() const
{
    return transform_;
}
