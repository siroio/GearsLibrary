#include <Internal/JoltPhysicsManager.h>
#include <Internal/ComponentManager.h>
#include <Internal/IRigidbody.h>
#include <InlineUtility.h>
#include <Vector3.h>
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


namespace
{
    Vector3 DEFAULT_GRAVITY{ 0, -9.81f, 0 };
    std::unique_ptr<JPH::TempAllocatorImpl> s_tempMemory;
    std::unique_ptr<JPH::JobSystemThreadPool> s_jobSystem;
    std::unique_ptr<JPH::PhysicsSystem> s_physicsSystem;
    std::unordered_map<uintptr_t, Glib::WeakPtr<Glib::Internal::Interface::IRigidbody>> s_rigidbodys;
    std::list<std::pair<GameObjectPtr, GameObjectPtr>> s_collisionEnterCallbacks;
    std::list<std::pair<GameObjectPtr, GameObjectPtr>> s_collisionStayCallbacks;
    std::list<std::pair<GameObjectPtr, GameObjectPtr>> s_collisionExitCallbacks;

    constexpr unsigned int MAX_RIGIDBODYS{ 65536 };
    constexpr unsigned int MAX_BODY_PAIR{ 65536 };
    constexpr unsigned int MAX_CONTACT_CONSTRANTS{ 10240 };
    constexpr unsigned int NUM_BODY_MUTEXS{ 0 };
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
        reinterpret_cast<const JPH::ObjectLayerPairFilter&>(*this)
    );

    s_physicsSystem->SetBodyActivationListener(this);
    s_physicsSystem->SetContactListener(this);
    s_physicsSystem->OptimizeBroadPhase();

    return true;
}

void Glib::Internal::Physics::JoltPhysicsManager::Finalize()
{
    JPH::UnregisterTypes();
    SafeDelete(JPH::Factory::sInstance);
}

void Glib::Internal::Physics::JoltPhysicsManager::Update()
{
    s_physicsSystem->Update(GameTimer::FixedDeltaTime(), 1, s_tempMemory.get(), s_jobSystem.get());

    // 衝突判定のコールバックを呼び出す
    ExecuteTriggerCallbacks();
    ExecuteCollisionCallbacks();
}


void Glib::Internal::Physics::JoltPhysicsManager::ExecuteTriggerCallbacks()
{

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
    return false;
}

JPH::uint Glib::Internal::Physics::JoltPhysicsManager::GetNumBroadPhaseLayers() const
{
    return JPH::uint();
}

JPH::BroadPhaseLayer Glib::Internal::Physics::JoltPhysicsManager::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
    return JPH::BroadPhaseLayer();
}

const char* Glib::Internal::Physics::JoltPhysicsManager::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
{
    return nullptr;
}

bool Glib::Internal::Physics::JoltPhysicsManager::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
{
    return false;
}

JPH::ValidateResult Glib::Internal::Physics::JoltPhysicsManager::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void Glib::Internal::Physics::JoltPhysicsManager::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{

}

void Glib::Internal::Physics::JoltPhysicsManager::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{

}

void Glib::Internal::Physics::JoltPhysicsManager::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{

}

void Glib::Internal::Physics::JoltPhysicsManager::OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{

}

void Glib::Internal::Physics::JoltPhysicsManager::OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{

}
