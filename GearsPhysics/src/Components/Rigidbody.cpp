#include <Components/Rigidbody.h>
#include <GameObject.h>
#include <Internal/BulletPhysicsManager.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <Vector3.h>
#include <Quaternion.h>
#include <memory>
#include <Debugger.h>
#include <GLGUI.h>
#include <imgui.h>
#include <Mathf.h>

namespace
{
    auto s_bullet = Glib::Internal::Physics::BulletPhysicsManager::Instance();
}

void Glib::Rigidbody::Start()
{
    const auto& transform = GameObject()->Transform();
    btTransform btTra{};
    btTra.setOrigin(ToVec3(transform->Position()));
    btTra.setRotation(ToQuat(transform->Rotation()));

    shape_ = std::make_unique<btBoxShape>(btVector3(btScalar(1.0f), btScalar(1.0f), btScalar(1.0f)));

    btVector3 localInertia(0, 0, 0);
    if (mass_ != 0.0f) shape_->calculateLocalInertia(mass_, localInertia);

    motionState_ = std::make_unique<btDefaultMotionState>(btTra);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass_, motionState_.get(), shape_.get(), localInertia);
    rigidbody_ = std::make_unique<btRigidBody>(rbInfo);
    rigidbody_->set
        if (rigidbody_->getMotionState() == nullptr)
        {
            Debug::Log("MotionState is nullptr");
        }

    s_bullet->AddRigidbody(rigidbody_.get());
    rigidbody_->setActivationState(ACTIVE_TAG);
}

void Glib::Rigidbody::Update()
{
    SyncTransform();
}

float Glib::Rigidbody::Mass() const
{
    return mass_;
}

void Glib::Rigidbody::Mass(float mass)
{
    mass_ = Mathf::Max(mass, 1.0e-7f);
}

bool Glib::Rigidbody::UseGravity() const
{
    return useGravity_;
}

void Glib::Rigidbody::UseGravity(bool useGravity)
{

}

void Glib::Rigidbody::OnGUI()
{

}

btVector3 Glib::Rigidbody::ToVec3(const Vector3& v)
{
    return btVector3{ -v.x, v.y, v.z };
}

btQuaternion Glib::Rigidbody::ToQuat(const Quaternion& v)
{
    return btQuaternion{ -v.x, v.y, v.z, v.w };
}

Vector3 Glib::Rigidbody::FromVec3(const btVector3& v)
{
    return Vector3{ -v.x(), v.y(), v.z() };
}

Quaternion Glib::Rigidbody::FromQuat(const btQuaternion& v)
{
    return Quaternion{ -v.x(), v.y(), v.z(), v.w() };
}

const GameObjectPtr& Glib::Rigidbody::GetGameObject()
{
    return GameObject();
}

void Glib::Rigidbody::SyncTransform()
{
    const auto& transform = GameObject()->Transform();
    btTransform worldTrans;
    motionState_->getWorldTransform(worldTrans);
    const auto& origin = worldTrans.getOrigin();
    const auto& rotation = worldTrans.getRotation();
    transform->Position(FromVec3(origin));
    transform->Rotation(FromQuat(rotation));
}
