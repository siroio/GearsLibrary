#include <GameObject.h>
#include <Component.h>
#include <Components/Transform.h>
#include <Internal/ImGuiInc.h>
#include <StringUtility.h>

GameObject::GameObject(std::string_view name) : name_{ name }
{}

GameObject::~GameObject()
{
    RemoveComponents();
}

void GameObject::Initialize()
{
    transform_ = AddComponent<Glib::Transform>();
}

void GameObject::RemoveComponents()
{
    components_.clear();
}

void GameObject::RemoveDeadComponents()
{
    std::erase_if(components_, [](const Glib::WeakPtr<Component>& component)
    {
        return component->IsDead();
    });
}

void GameObject::DrawGUI()
{
    for (const auto& component : components_)
    {
        std::string name = Glib::nameof(*component.get());
        ImGui::PushID(component.get());
        ImGui::Separator();
        if (ImGui::CollapsingHeader(name.data()))
        {
            component->OnGUI();
        }
        ImGui::PopID();
    }
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
    const auto& parent = transform_->Parent();
    return isActive_ && (parent.expired() || parent->GameObject()->Active());
}

void GameObject::Active(bool active)
{
    isActive_ = active;
}

void GameObject::Name(std::string_view name)
{
    name_ = name;
}

std::string GameObject::Name() const
{
    return name_;
}

void GameObject::Tag(std::string_view tag)
{
    tag_ = tag;
}

std::string GameObject::Tag() const
{
    return tag_;
}

unsigned int GameObject::Layer() const
{
    return layer_;
}

void GameObject::Layer(unsigned int layer)
{
    layer_ = layer;
}

bool GameObject::IsDead() const
{
    return isDead_;
}

bool GameObject::IsRoot() const
{
    return transform_->Parent().expired();
}

const Glib::WeakPtr<Glib::Transform>& GameObject::Transform() const
{
    return transform_;
}
