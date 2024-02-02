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

    if (rigidbody_->getMotionState() == nullptr)
    {
        Debug::Log("MotionState is nullptr");
        return;
    }

    // 自身のポインタをセット
    rigidbody_->setUserPointer(GameObject().get().get());

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
    mass_ = Mathf::Max(mass, 0.0f);
    if (rigidbody_ == nullptr) return;
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if (mass_ != 0.0f) shape_->calculateLocalInertia(mass_, localInertia);
    rigidbody_->setMassProps(mass_, localInertia);
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
    auto gravity = FromVec3(rigidbody_->getGravity());
    if (GLGUI::DragVector3("Gravity", &gravity, 0.1f))
    {
        rigidbody_->setGravity(ToVec3(gravity));
    }
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

void Glib::Rigidbody::SyncToBullet()
{
    const auto& transform = GameObject()->Transform();
    btTransform worldTrans;
    worldTrans.setOrigin(ToVec3(transform->Position()));
    worldTrans.setRotation(ToQuat(transform->Rotation()));
    motionState_->setWorldTransform(worldTrans);
}

void Glib::Rigidbody::SyncFromBullet()
{
    const auto& transform = GameObject()->Transform();
    btTransform worldTrans;
    motionState_->getWorldTransform(worldTrans);
    const auto& origin = worldTrans.getOrigin();
    const auto& rotation = worldTrans.getRotation();
    transform->Position(FromVec3(origin));
    transform->Rotation(FromQuat(rotation));
}

const GameObjectPtr& Glib::Rigidbody::GetGameObject()
{
    return GameObject();
}

btRigidBody* Glib::Rigidbody::GetbtRigidbody()
{
    return rigidbody_.get();
}
