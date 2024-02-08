#include <Internal/JoltPhysicsManager.h>
#include <Internal/ComponentManager.h>
#include <Internal/IRigidbody.h>
#include <InlineUtility.h>
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix4x4.h>
#include <GameTimer.h>
#include <GameObject.h>
#include <Debugger.h>
#include <unordered_map>
#include <memory>
#include <list>
#include <thread>

/* JoltPhysics */
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>



namespace
{
    Vector3 DEFAULT_GRAVITY{ 0, -9.81f, 0 };

    std::unique_ptr<JPH::TempAllocatorImpl> s_tempMemory;
    std::unique_ptr<JPH::JobSystemThreadPool> s_jobSystem;
    std::unique_ptr<JPH::PhysicsSystem> s_physicsSystem;

    std::unordered_map<unsigned int, unsigned int> s_physicsLayers;
    std::unordered_map<uintptr_t, Glib::WeakPtr<Glib::Internal::Interface::IRigidbody>> s_rigidbodys;

    using GameObjectPair = std::pair<GameObjectPtr, GameObjectPtr>;

    std::list<GameObjectPair> s_triggerEnterCallbacks;
    std::list<GameObjectPair> s_triggerStayCallbacks;
    std::list<GameObjectPair> s_triggerExitCallbacks;

    std::list<GameObjectPair> s_collisionEnterCallbacks;
    std::list<GameObjectPair> s_collisionStayCallbacks;
    std::list<GameObjectPair> s_collisionExitCallbacks;


    constexpr unsigned int MAX_RIGIDBODYS{ 65536 };
    constexpr unsigned int MAX_BODY_PAIR{ 65536 };
    constexpr unsigned int MAX_CONTACT_CONSTRANTS{ 10240 };
    constexpr unsigned int NUM_BODY_MUTEXS{ 0 };

    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
}

namespace
{
    auto s_componentManager = Glib::Internal::ComponentManager::Instance();
}

bool Glib::Internal::Physics::JoltPhysicsManager::Initialize()
{
    JPH::RegisterDefaultAllocator();

    JPH::Factory::sInstance = new JPH::Factory{};
    if (JPH::Factory::sInstance == nullptr) return false;

    JPH::RegisterTypes();

    // メモリアロケータ作成
    s_tempMemory = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);

    // JobSystem用のスレッドプール作成
    s_jobSystem = std::make_unique<JPH::JobSystemThreadPool>(
        JPH::cMaxPhysicsJobs,
        JPH::cMaxPhysicsBarriers,
        std::thread::hardware_concurrency() - 1
    );

    s_physicsSystem = std::make_unique<JPH::PhysicsSystem>();
    if (s_physicsSystem.get() == nullptr) return false;
    s_physicsSystem->Init(
        MAX_RIGIDBODYS,
        NUM_BODY_MUTEXS,
        MAX_BODY_PAIR,
        MAX_CONTACT_CONSTRANTS,
        static_cast<const JPH::BroadPhaseLayerInterface&>(*this),
        static_cast<const JPH::ObjectVsBroadPhaseLayerFilter&>(*this),
        static_cast<const JPH::ObjectLayerPairFilter&>(*this)
    );

    s_physicsSystem->SetBodyActivationListener(this);
    s_physicsSystem->SetContactListener(this);
    s_physicsSystem->OptimizeBroadPhase();

    s_physicsSystem->SetGravity(ToRVec3(DEFAULT_GRAVITY));

    return true;
}

void Glib::Internal::Physics::JoltPhysicsManager::Finalize()
{
    JPH::UnregisterTypes();
    SafeDelete(JPH::Factory::sInstance);
}

void Glib::Internal::Physics::JoltPhysicsManager::Update()
{
    for (const auto& [_, rigidbody] : s_rigidbodys)
    {
        rigidbody->SyncToPhysics();
    }

    s_physicsSystem->Update(GameTimer::FixedDeltaTime(), 1, s_tempMemory.get(), s_jobSystem.get());

    for (const auto& [_, rigidbody] : s_rigidbodys)
    {
        rigidbody->SyncFromPhysics();
    }

    // 衝突判定のコールバックを呼び出す
    ExecuteTriggerCallbacks();
    ExecuteCollisionCallbacks();
}


void Glib::Internal::Physics::JoltPhysicsManager::ExecuteTriggerCallbacks()
{
    for (const auto& [body1, body2] : s_triggerEnterCallbacks)
    {
        s_componentManager->ExecuteEventFunction(
            body1,
            ComponentFunctionType::OnTriggerEnter,
            body2
        );
    }

    for (const auto& [body1, body2] : s_triggerStayCallbacks)
    {
        s_componentManager->ExecuteEventFunction(
            body1,
            ComponentFunctionType::OnTriggerStay,
            body2
        );
    }

    s_triggerStayCallbacks.splice(
        s_triggerStayCallbacks.end(),
        std::move(s_triggerEnterCallbacks)
    );

    for (const auto& [body1, body2] : s_triggerExitCallbacks)
    {
        s_componentManager->ExecuteEventFunction(
            body1,
            ComponentFunctionType::OnCollisionExit,
            body2
        );
    }

    s_triggerEnterCallbacks.clear();
    s_triggerExitCallbacks.clear();
}

void Glib::Internal::Physics::JoltPhysicsManager::ExecuteCollisionCallbacks()
{
    for (const auto& [body1, body2] : s_collisionEnterCallbacks)
    {
        s_componentManager->ExecuteEventFunction(
            body1,
            ComponentFunctionType::OnCollisionEnter,
            body2
        );
    }

    for (const auto& [body1, body2] : s_collisionStayCallbacks)
    {
        s_componentManager->ExecuteEventFunction(
            body1,
            ComponentFunctionType::OnCollisionStay,
            body2
        );
    }

    s_collisionStayCallbacks.splice(
        s_collisionStayCallbacks.end(),
        std::move(s_collisionEnterCallbacks)
    );

    for (const auto& [body1, body2] : s_collisionExitCallbacks)
    {
        s_componentManager->ExecuteEventFunction(
            body1,
            ComponentFunctionType::OnCollisionExit,
            body2
        );
    }

    s_collisionEnterCallbacks.clear();
    s_collisionExitCallbacks.clear();
}

bool Glib::Internal::Physics::JoltPhysicsManager::Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* hit)
{
    return false;
}

bool Glib::Internal::Physics::JoltPhysicsManager::RaycastAll(const Vector3& origin, const Vector3& direction, float maxDistance, std::vector<RaycastHit>* hit)
{
    return false;
}

bool Glib::Internal::Physics::JoltPhysicsManager::AddRigidbody(const WeakPtr<Glib::Internal::Interface::IRigidbody>& rigidbody)
{
    if (rigidbody.expired()) return false;
    auto& bodyInterface = s_physicsSystem->GetBodyInterface();
    bodyInterface.AddBody(rigidbody->GetBodyID(), JPH::EActivation::Activate);
    s_rigidbodys.emplace(rigidbody.getId(), rigidbody);
    return true;
}

JPH::Body* Glib::Internal::Physics::JoltPhysicsManager::CreateBody(const JPH::BodyCreationSettings& bodySettings)
{
    auto& bodyInterface = s_physicsSystem->GetBodyInterface();
    return bodyInterface.CreateBody(bodySettings);
}

void Glib::Internal::Physics::JoltPhysicsManager::RemoveRigidbody(const WeakPtr<Interface::IRigidbody>& rigidbody)
{
    auto& bodyInterface = s_physicsSystem->GetBodyInterface();
    bodyInterface.RemoveBody(rigidbody->GetBodyID());
    bodyInterface.DestroyBody(rigidbody->GetBodyID());
}

JPH::BodyInterface& Glib::Internal::Physics::JoltPhysicsManager::GetBodyInterface() const
{
    return s_physicsSystem->GetBodyInterface();
}

Matrix4x4 Glib::Internal::Physics::JoltPhysicsManager::ToMatrix4x4(const JPH::RMat44& mat)
{
    return Matrix4x4{
        mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3),
        mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3),
        mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3),
        mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3),
    };
}

JPH::RVec3 Glib::Internal::Physics::JoltPhysicsManager::ToRVec3(const Vector3& v)
{
    return JPH::RVec3{ v.x, v.y, v.z };
}

JPH::Quat Glib::Internal::Physics::JoltPhysicsManager::ToQuat(const Quaternion& qua)
{
    return JPH::Quat{ qua.x, qua.y, qua.z, qua.w };
}

JPH::uint Glib::Internal::Physics::JoltPhysicsManager::GetNumBroadPhaseLayers() const
{
    Debug::Log("GetNumBroadPhaseLayers");
    return BroadPhaseLayers::NUM_LAYERS;
}

JPH::BroadPhaseLayer Glib::Internal::Physics::JoltPhysicsManager::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
    // 物体のレイヤーに対応するブロードフェーズのレイヤーを返す
    return mObjectToBroadPhase[inLayer];
}

const char* Glib::Internal::Physics::JoltPhysicsManager::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
{
    Debug::Log("GetBroadPhaseLayerName");
    switch ((JPH::BroadPhaseLayer::Type)inLayer)
    {
        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
            return "NON_MOVING";
        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
            return "MOVING";
        default:
            JPH_ASSERT(false);
            return "INVALID";
    }
    return "INVALID";
}

bool Glib::Internal::Physics::JoltPhysicsManager::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const
{
    switch (inLayer1)
    {
        case Layers::NON_MOVING:
            return inLayer2 == Layers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
    }
    return false;
}

bool Glib::Internal::Physics::JoltPhysicsManager::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
{
    Debug::Log("ShouldCollide");
    switch (inLayer1)
    {
        case Layers::NON_MOVING:
            return inLayer2 == BroadPhaseLayers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
    }
    return false;
}

JPH::ValidateResult Glib::Internal::Physics::JoltPhysicsManager::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
    Debug::Log("OnContactValidate");
    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void Glib::Internal::Physics::JoltPhysicsManager::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
    Debug::Log("OnContactAdded");

    const auto& object1 = s_rigidbodys.find(static_cast<uintptr_t>(inBody1.GetUserData()));
    const auto& object2 = s_rigidbodys.find(static_cast<uintptr_t>(inBody2.GetUserData()));
    if (object1 == s_rigidbodys.end() || object2 == s_rigidbodys.end()) return;
    const auto& rb1 = object1->second;
    const auto& rb2 = object2->second;
    s_collisionEnterCallbacks.push_back({ rb1->GetGameObject(), rb2->GetGameObject() });
    s_collisionEnterCallbacks.push_back({ rb2->GetGameObject(), rb1->GetGameObject() });
}

void Glib::Internal::Physics::JoltPhysicsManager::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
    Debug::Log("OnContactPersisted");

    const auto& object1 = s_rigidbodys.find(static_cast<uintptr_t>(inBody1.GetUserData()));
    const auto& object2 = s_rigidbodys.find(static_cast<uintptr_t>(inBody2.GetUserData()));
    if (object1 == s_rigidbodys.end() || object2 == s_rigidbodys.end()) return;

    const auto& rb1 = object1->second;
    const auto& rb2 = object2->second;
    s_collisionStayCallbacks.push_back({ rb1->GetGameObject(), rb2->GetGameObject() });
    s_collisionStayCallbacks.push_back({ rb2->GetGameObject(), rb1->GetGameObject() });
}

void Glib::Internal::Physics::JoltPhysicsManager::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
    Debug::Log("OnContactRemoved");

}

void Glib::Internal::Physics::JoltPhysicsManager::OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{
    Debug::Log("OnBodyActivated");
}

void Glib::Internal::Physics::JoltPhysicsManager::OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{
    Debug::Log("OnBodyDeactivated");
}
