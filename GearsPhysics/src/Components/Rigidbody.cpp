﻿#include <Components/Rigidbody.h>
#include <GameObject.h>
#include <Internal/PhysXManager.h>
#include <Quaternion.h>
#include <memory>
#include <Debugger.h>
#include <GLGUI.h>

namespace
{
    auto s_physX = Glib::Internal::Physics::PhysXManager::Instance();
    std::vector<std::string> s_combo{ "Discrete", "Continuous", "Continuous Dynamic", "Continuous Speculative" };
}

Glib::Rigidbody::~Rigidbody()
{
    if (rigidDynamic_ != nullptr)
    {
        s_physX->RemoveActor(rigidDynamic_);
        rigidDynamic_ = nullptr;
    }
}

void Glib::Rigidbody::Start()
{
    const auto& transform = GameObject()->Transform();
    rigidDynamic_ = static_cast<physx::PxRigidDynamic*>(s_physX->CreateRigidBody(
        transform->Position(),
        transform->Rotation(),
        WeakPtr<Rigidbody>{ shared_from_this() }
    ));

    rigidDynamic_->setMass(mass_); // 質量
    rigidDynamic_->setLinearDamping(linearDamping_); // 空気抵抗
    rigidDynamic_->setAngularDamping(angularDamping_); // 回転の空気抵抗
    rigidDynamic_->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity_); // 重力
    rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic_);
    rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue())); // 制限

    if (!isKinematic_)
    {
        const auto& linear = Internal::Physics::PhysXManager::ToPxVec3(linearVelocity_);
        const auto& angular = Internal::Physics::PhysXManager::ToPxVec3(angularVelocity_);
        rigidDynamic_->setLinearVelocity(linear);
        rigidDynamic_->setAngularVelocity(angular);
    }

    if (rigidDynamic_ == nullptr)
    {
        Debug::Log("Rigidbody could not be added.");
        Destroy();
    }
}

void Glib::Rigidbody::AddForce(const Vector3& force, ForceMode mode)
{
    if (rigidDynamic_ == nullptr) return;
    rigidDynamic_->addForce(
        Internal::Physics::PhysXManager::ToPxVec3(force),
        static_cast<physx::PxForceMode::Enum>(mode)
    );
}

void Glib::Rigidbody::AddTorque(const Vector3& torque, ForceMode mode)
{
    if (rigidDynamic_ == nullptr) return;
    rigidDynamic_->addTorque(
        Internal::Physics::PhysXManager::ToPxVec3(torque),
        static_cast<physx::PxForceMode::Enum>(mode)
    );
}

const Glib::BitFlag<Glib::RigidbodyConstraints>& Glib::Rigidbody::Constraints() const
{
    return constrants_;
}

void Glib::Rigidbody::Constraints(const RigidbodyConstraints& constraints)
{
    constrants_.Copy(static_cast<BitFlag<RigidbodyConstraints>::EnumType>(constraints));

    if (rigidDynamic_ != nullptr)
    {
        rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
    }
}

bool Glib::Rigidbody::IsKinematic() const
{
    return isKinematic_;
}

void Glib::Rigidbody::IsKinematic(bool enable)
{
    isKinematic_ = enable;
    if (rigidDynamic_ != nullptr) rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, enable);
}

float Glib::Rigidbody::Mass() const
{
    return rigidDynamic_ != nullptr ? rigidDynamic_->getMass() : 0.0f;
}

void Glib::Rigidbody::Mass(float mass)
{
    mass_ = mass;
    if (rigidDynamic_ != nullptr) rigidDynamic_->setMass(mass);
}

Vector3 Glib::Rigidbody::LinearVelocity() const
{
    if (rigidDynamic_ == nullptr) return Vector3::Zero();
    return Internal::Physics::PhysXManager::ToVector3(rigidDynamic_->getLinearVelocity());
}

void Glib::Rigidbody::LinearVelocity(const Vector3& linearVelocity)
{
    linearVelocity_ = linearVelocity;
    if (rigidDynamic_ != nullptr)
    {
        const auto& linearV = Internal::Physics::PhysXManager::ToPxVec3(linearVelocity);
        rigidDynamic_->setLinearVelocity(linearV);
    }
}

Vector3 Glib::Rigidbody::AngularVelocity() const
{
    if (rigidDynamic_ == nullptr) return Vector3::Zero();
    return Internal::Physics::PhysXManager::ToVector3(rigidDynamic_->getAngularVelocity());
}

void Glib::Rigidbody::AngularVelocity(const Vector3& angularVelocity)
{
    angularVelocity_ = angularVelocity;
    if (rigidDynamic_ != nullptr)
    {
        const auto& angularV = Internal::Physics::PhysXManager::ToPxVec3(angularVelocity);
        rigidDynamic_->setAngularVelocity(angularV);
    }
}

float Glib::Rigidbody::LinearDamping() const
{
    if (rigidDynamic_ == nullptr) return 0.0f;
    return rigidDynamic_->getLinearDamping();
}

void Glib::Rigidbody::LinearDamping(float linearDamping)
{
    linearDamping_ = linearDamping;
    if (rigidDynamic_ != nullptr)
    {
        rigidDynamic_->setLinearDamping(linearDamping);
    }
}

float Glib::Rigidbody::AngularDamping() const
{
    if (rigidDynamic_ == nullptr) return 0.0f;
    return rigidDynamic_->getAngularDamping();
}

void Glib::Rigidbody::AngularDamping(float angularDamping)
{
    angularDamping_ = angularDamping;
    if (rigidDynamic_ != nullptr)
    {
        rigidDynamic_->setAngularDamping(angularDamping);
    }
}

bool Glib::Rigidbody::UseGravity() const
{
    return useGravity_;
}

void Glib::Rigidbody::UseGravity(bool useGravity)
{
    useGravity_ = useGravity;
    if (rigidDynamic_ != nullptr) rigidDynamic_->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity_);
}

void Glib::Rigidbody::OnGUI()
{
    Component::OnGUI();

    if (GLGUI::DragFloat("Mass", &mass_, 0.01f, 0.0f, PX_MAX_F32))
    {
        Mass(mass_);
    }

    if (GLGUI::DragFloat("Linear Damping", &linearDamping_, 0.01f, 0.0f, PX_MAX_F32))
    {
        LinearDamping(linearDamping_);
    }

    if (GLGUI::DragFloat("Angular Damping", &angularDamping_, 0.01f, 0.0f, PX_MAX_F32))
    {
        AngularDamping(angularDamping_);
    }

    if (GLGUI::CheckBox("Use Gravity", &useGravity_))
    {
        rigidDynamic_->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity_);
    }

    if (GLGUI::CheckBox("Is Kinematic", &isKinematic_))
    {
        rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic_);
    }

    if (GLGUI::ComboBox("Detection Mode", currentMode_, s_combo))
    {
        const auto& it = std::ranges::find(s_combo, currentMode_);
        const auto& idx = std::distance(s_combo.begin(), it);
        detectionMode_ = static_cast<CDMODE>(idx);
        DetectionMode(detectionMode_);
    }

    if (GLGUI::TreeNode("Freeze Position"))
    {
        bool freezePosX = constrants_.Has(RigidbodyConstraints::FreezePositionX);
        bool freezePosY = constrants_.Has(RigidbodyConstraints::FreezePositionY);
        bool freezePosZ = constrants_.Has(RigidbodyConstraints::FreezePositionZ);

        if (GLGUI::CheckBox("X", &freezePosX))
        {
            constrants_.Set(RigidbodyConstraints::FreezePositionX, freezePosX);
            rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
        }
        if (GLGUI::CheckBox("Y", &freezePosY, true))
        {
            constrants_.Set(RigidbodyConstraints::FreezePositionY, freezePosY);
            rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
        }
        if (GLGUI::CheckBox("Z", &freezePosZ, true))
        {
            constrants_.Set(RigidbodyConstraints::FreezePositionZ, freezePosZ);
            rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
        }

        GLGUI::TreePop();
    }

    if (GLGUI::TreeNode("Freeze Rotation"))
    {
        bool freezeRotX = constrants_.Has(RigidbodyConstraints::FreezeRotationX);
        bool freezeRotY = constrants_.Has(RigidbodyConstraints::FreezeRotationY);
        bool freezeRotZ = constrants_.Has(RigidbodyConstraints::FreezeRotationZ);

        if (GLGUI::CheckBox("X", &freezeRotX))
        {
            constrants_.Set(RigidbodyConstraints::FreezeRotationX, freezeRotX);
            rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
        }
        if (GLGUI::CheckBox("Y", &freezeRotY, true))
        {
            constrants_.Set(RigidbodyConstraints::FreezeRotationY, freezeRotY);
            rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
        }
        if (GLGUI::CheckBox("Z", &freezeRotZ, true))
        {
            constrants_.Set(RigidbodyConstraints::FreezeRotationZ, freezeRotZ);
            rigidDynamic_->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(constrants_.ToValue()));
        }

        GLGUI::TreePop();
    }
}

Glib::CDMODE Glib::Rigidbody::DetectionMode() const
{
    return detectionMode_;
}

void Glib::Rigidbody::DetectionMode(CDMODE mode)
{
    switch (mode)
    {
        case CDMODE::Discrete:
            detectionMode_ = CDMODE::Discrete;
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD_FRICTION, false);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
            return;
        case CDMODE::Continuous:
            detectionMode_ = CDMODE::Continuous;
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD_FRICTION, false);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
            return;
        case CDMODE::ContinuousDynamic:
            detectionMode_ = CDMODE::ContinuousDynamic;
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD_FRICTION, true);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
            return;
        case CDMODE::ContinuousSpeculative:
            detectionMode_ = CDMODE::ContinuousSpeculative;
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD_FRICTION, false);
            rigidDynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
            return;
    }
}

void Glib::Rigidbody::SyncToPhysics()
{
    const auto& transform = GameObject()->Transform();
    const auto& position = Internal::Physics::PhysXManager::ToPxVec3(transform->Position());
    const auto& rotation = Internal::Physics::PhysXManager::ToPxQuat(transform->Rotation());
    physx::PxTransform syncTrans{ position, rotation };
    rigidDynamic_->setGlobalPose(syncTrans);
    rigidDynamic_->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !Active());
}

void Glib::Rigidbody::SyncFromPhysics()
{
    if (isKinematic_) return;
    const auto& transform = GameObject()->Transform();
    physx::PxTransform syncTrans = rigidDynamic_->getGlobalPose();
    const auto& position = Internal::Physics::PhysXManager::ToVector3(syncTrans.p);
    const auto& rotation = Internal::Physics::PhysXManager::ToQuaternion(syncTrans.q);
    transform->Position(position);
    transform->Rotation(rotation);
}

physx::PxRigidDynamic& Glib::Rigidbody::GetRigidDynamic()
{
    return *rigidDynamic_;
}

const GameObjectPtr& Glib::Rigidbody::GetGameObject()
{
    return GameObject();
}
