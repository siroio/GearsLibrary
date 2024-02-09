#include <Components/Rigidbody.h>
#include <GameObject.h>
#include <Internal/JoltPhysicsManager.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <Quaternion.h>
#include <memory>
#include <Debugger.h>
#include <GLGUI.h>
#include <imgui.h>
#include <Mathf.h>

#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace
{
    auto s_joltPhysics = Glib::Internal::Physics::JoltPhysicsManager::Instance();
}

void Glib::Rigidbody::Start()
{
    JPH::BoxShapeSettings shape{ { 100.0f, 1.0f, 100.0f } };
    JPH::ShapeSettings::ShapeResult shapeRes = shape.Create();
    JPH::ShapeRefC testShape = shapeRes.Get();

    if (GameObject()->Name() == "Ground")
    {
        JPH::BodyCreationSettings settings{ testShape, { 0.0, -3.0, 0.0 }, JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING };
        body_ = s_joltPhysics->CreateBody(settings);
    }
    else
    {
        JPH::BodyCreationSettings settings{ testShape, { 0.0, 2.0, 0.0 }, JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING };
        body_ = s_joltPhysics->CreateBody(settings);
    }

    bool result = s_joltPhysics->AddRigidbody(WeakPtr<Rigidbody>{ shared_from_this() });
    if (!result)
    {
        Debug::Log("Rigidbody could not be added.");
        Destroy();
    }
}

void Glib::Rigidbody::OnDestroy()
{
    s_joltPhysics->RemoveRigidbody(WeakPtr<Rigidbody>{ shared_from_this() });
}

float Glib::Rigidbody::Mass() const
{
    return mass_;
}

void Glib::Rigidbody::Mass(float mass)
{
    mass_ = mass;
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

void Glib::Rigidbody::SyncToPhysics()
{

    const auto& transform = GameObject()->Transform();
    const auto position = Internal::Physics::JoltPhysicsManager::ToRVec3(transform->Position());
    const auto rotation = Internal::Physics::JoltPhysicsManager::ToQuat(transform->Rotation());

    auto& bodyInterface = s_joltPhysics->GetBodyInterface();
    bodyInterface.SetPositionAndRotation(GetBodyID(), position, rotation.Normalized(), JPH::EActivation::DontActivate);
}

void Glib::Rigidbody::SyncFromPhysics()
{
    auto& bodyInterface = s_joltPhysics->GetBodyInterface();
    const auto& comMat = bodyInterface.GetCenterOfMassTransform(GetBodyID());
    const Matrix4x4 worldMat = Internal::Physics::JoltPhysicsManager::ToMatrix4x4(comMat);

    const auto& transform = GameObject()->Transform();
    transform->Position(worldMat.Position());
    transform->Rotation(worldMat.Rotation());
}

const GameObjectPtr& Glib::Rigidbody::GetGameObject()
{
    return GameObject();
}

const JPH::BodyID& Glib::Rigidbody::GetBodyID()
{
    return body_->GetID();
}
