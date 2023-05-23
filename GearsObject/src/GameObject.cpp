#include <GameObject.h>
#include <Transform.h>

GameObject::GameObject(std::string_view name) : name_{ name }
{}

GameObject::~GameObject()
{
    RemoveComponents();
}

bool GameObject::IsAcive() const
{
    return isActive_;
}

void GameObject::Active(bool active)
{
    isActive_ = active;
}

void GameObject::Initialize()
{}

void GameObject::RemoveComponents()
{}

void GameObject::RemoveDeadComponents()
{}

void GameObject::Destroy()
{}

bool GameObject::DontDestroyOnLoad() const
{
    return isDontDestroyOnLoad_;
}

void GameObject::DontDestroyOnLoad(bool dontDestroyOnLoad)
{
    isDontDestroyOnLoad_ = dontDestroyOnLoad;
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
