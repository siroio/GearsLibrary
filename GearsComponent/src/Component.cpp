#include <Component.h>
#include <GameObject.h>
#include <GLGUI.h>

bool Component::Active() const
{
    return isActive_ && !gameObject_.expired() && gameObject_->Active();
}

void Component::Active(bool active)
{
    isActive_ = active;
}

const GameObjectPtr& Component::GameObject() const
{
    return gameObject_;
}

void Component::Destroy()
{
    isActive_ = false;
    isDead_ = true;
}

bool Component::IsDead() const
{
    return isDead_;
}

void Component::OnGUI()
{
    Glib::GLGUI::CheckBox("Active", &isActive_);
}

void Component::SetGameObject(const GameObjectPtr& gameObject)
{
    if (gameObject_.expired())
    {
        gameObject_ = gameObject;
    }
}
