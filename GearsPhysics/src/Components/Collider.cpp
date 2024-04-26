#include <Components/Collider.h>
#include <Internal/PhysXManager.h>
#include <Components/Rigidbody.h>
#include <GameObject.h>
#include <PxPhysicsAPI.h>
#include <Mathf.h>
#include <Debugger.h>
#include <GLGUI.h>
#include <utility>

namespace
{
    auto s_physX = Glib::Internal::Physics::PhysXManager::Instance();
    std::vector<std::string> combine{ "Average", "Min", "Multiply", "Max" };
}

Glib::Collider::~Collider()
{
    if (material_ != nullptr)
    {
        material_->release();
        material_ = nullptr;
    }

    if (!rigidbody_.expired() && shape_ != nullptr)
    {
        shape_->release();
        shape_ = nullptr;
    }

    if (rigidStatic_ != nullptr)
    {
        rigidStatic_->release();
        rigidStatic_ = nullptr;
    }
}

bool Glib::Collider::IsTrigger() const
{
    return isTrigger_;
}

void Glib::Collider::IsTrigger(bool enable)
{
    isTrigger_ = enable;
    if (shape_ != nullptr) SetTrigger();
}

bool Glib::Collider::IsVisible() const
{
    return isVisible_;
}

void Glib::Collider::IsVisible(bool enable)
{
    isVisible_ = enable;
    if (shape_ != nullptr) SetVisible();
}

const Vector3& Glib::Collider::Center() const
{
    return center_;
}

void Glib::Collider::Center(const Vector3& center)
{
    center_ = center;
    if (shape_ != nullptr)
    {
        auto transform = shape_->getLocalPose();
        transform.p = Internal::Physics::PhysXManager::ToPxVec3(center);
        shape_->setLocalPose(transform);
    }
}

float Glib::Collider::DynamicFriction() const
{
    return dynamicFriction_;
}

void Glib::Collider::DynamicFriction(float friction)
{
    dynamicFriction_ = friction;
    if (material_ != nullptr)
    {
        material_->setDynamicFriction(friction);
    }
}

float Glib::Collider::StaticFriction() const
{
    return staticFriction_;
}

void Glib::Collider::StaticFriction(float friction)
{
    staticFriction_ = friction;
    if (material_ != nullptr)
    {
        material_->setStaticFriction(friction);
    }
}

float Glib::Collider::Bounciness() const
{
    return bounciness_;
}

void Glib::Collider::Bounciness(float bounce)
{
    bounciness_ = Mathf::Clamp01(bounce);
    if (material_ != nullptr)
    {
        material_->setRestitution(bounce);
    }
}

void Glib::Collider::SetFrictionCombine(Combine combine)
{
    frictionCombine_ = combine;
    if (shape_ != nullptr)
    {
        material_->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(combine));
    }
}

void Glib::Collider::SetBounceCombine(Combine combine)
{
    bounceCombine_ = combine;
    if (shape_ != nullptr)
    {
        material_->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(combine));
    }
}

bool Glib::Collider::Initialize()
{
    rigidbody_ = GameObject()->GetComponent<Glib::Rigidbody>();
    return !rigidbody_.expired();
}

void Glib::Collider::CreateShape(const physx::PxGeometry& geometry)
{
    material_ = s_physX->CreateMaterial(dynamicFriction_, staticFriction_, bounciness_);
    physx::PxRigidActor* actor = rigidbody_.expired() ?
        static_cast<physx::PxRigidActor*>(rigidStatic_) :
        &rigidbody_->GetRigidDynamic();
    shape_ = physx::PxRigidActorExt::createExclusiveShape(*actor, geometry, *material_);
    shape_->setFlag(physx::PxShapeFlag::eVISUALIZATION, isVisible_);

    physx::PxFilterData filter{};
    filter.word0 = GameObject()->Layer();
    shape_->setSimulationFilterData(filter);
    s_physX->AddCollider(WeakPtr<Collider>{ weak_from_this() });
}

physx::PxShape* Glib::Collider::Shape()
{
    return shape_;
}

void Glib::Collider::OnGUI()
{
    Component::OnGUI();

    if (GLGUI::CheckBox("Is Trigger", &isTrigger_))
    {
        SetTrigger();
    }

    if (GLGUI::CheckBox("Is Visible", &isVisible_))
    {
        SetVisible();
    }

    if (GLGUI::TreeNode("Physic Material"))
    {
        if (GLGUI::DragFloat("Dynamic Friction", &dynamicFriction_, 0.01f))
        {
            DynamicFriction(dynamicFriction_);
        }

        if (GLGUI::DragFloat("Static Friction", &staticFriction_, 0.01f))
        {
            StaticFriction(staticFriction_);
        }

        if (GLGUI::DragFloat("Bounciness", &bounciness_, 0.01f))
        {
            Bounciness(bounciness_);
        }

        std::string currentCombine = combine[static_cast<int>(frictionCombine_)];
        if (GLGUI::ComboBox("FrictionCombine", currentCombine, combine))
        {
            auto it = std::find(combine.begin(), combine.end(), currentCombine);
            frictionCombine_ = static_cast<Glib::Combine>(std::distance(combine.begin(), it));
        }

        currentCombine = combine[static_cast<int>(bounceCombine_)];
        if (GLGUI::ComboBox("BouncinessCombine", currentCombine, combine))
        {
            auto it = std::find(combine.begin(), combine.end(), currentCombine);
            bounceCombine_ = static_cast<Glib::Combine>(std::distance(combine.begin(), it));
        }
        GLGUI::TreePop();
    }

    Vector3 center = Center();
    if (GLGUI::DragVector3("Center", &center, 0.01f))
    {
        Center(center);
    }
}

void Glib::Collider::SetTrigger()
{
    // trigger, simulationフラグの切り替え
    auto disable = physx::PxShapeFlag::eTRIGGER_SHAPE;
    auto enable = physx::PxShapeFlag::eSIMULATION_SHAPE;

    if (isTrigger_) std::swap(disable, enable);

    shape_->setFlag(disable, false);
    shape_->setFlag(enable, Active());
}

void Glib::Collider::SetVisible()
{
    shape_->setFlag(physx::PxShapeFlag::eVISUALIZATION, isVisible_ && Active());
}

void Glib::Collider::SyncActive()
{
    shape_->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, Active());
    SetTrigger();
    SetVisible();
}
