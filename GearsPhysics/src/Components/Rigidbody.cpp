#include <Components/Rigidbody.h>
#include <GameObject.h>
#include <Internal/JoltPhysicsManager.h>
#include <Vector3.h>
#include <Quaternion.h>
#include <memory>
#include <Debugger.h>
#include <GLGUI.h>
#include <imgui.h>
#include <Mathf.h>

namespace
{
    auto s_bullet = Glib::Internal::Physics::JoltPhysicsManager::Instance();
}

void Glib::Rigidbody::Start()
{


    bool result = s_bullet->AddRigidbody(WeakPtr<Rigidbody>{ shared_from_this() });
    if (!result)
    {
        Debug::Log("Rigidbody could not be added.");
        Destroy();
    }
}

float Glib::Rigidbody::Mass() const
{
    return mass_;
}

void Glib::Rigidbody::Mass(float mass)
{

}

bool Glib::Rigidbody::UseGravity() const
{
    return useGravity_;
}

void Glib::Rigidbody::UseGravity(bool useGravity)
{
    useGravity_ = useGravity;
}

void Glib::Rigidbody::OnGUI()
{
    Component::OnGUI();
}

void Glib::Rigidbody::SyncToBullet()
{

}

void Glib::Rigidbody::SyncFromBullet()
{}

const GameObjectPtr& Glib::Rigidbody::GetGameObject()
{
    return GameObject();
}
