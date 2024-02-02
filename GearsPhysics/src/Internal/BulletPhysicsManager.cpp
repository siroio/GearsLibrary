#include <Internal/BulletPhysicsManager.h>
#include <Internal/ComponentManager.h>
#include <Internal/IRigidbody.h>
#include <btBulletDynamicsCommon.h>
#include <Vector3.h>
#include <GameTimer.h>
#include <GameObject.h>
#include <Debugger.h>
#include <unordered_map>
#include <memory>
#include <deque>

namespace
{
    Vector3 DEFAULT_GRAVITY{ 0, -9.81f, 0 };
    unsigned int SIMULATE_SUB_STEP{ 10 };
    std::unique_ptr<btBroadphaseInterface> s_broadphase{ nullptr };
    std::unique_ptr<btDefaultCollisionConfiguration> s_configuration{ nullptr };
    std::unique_ptr<btCollisionDispatcher> s_dispatcher{ nullptr };
    std::unique_ptr<btSequentialImpulseConstraintSolver> s_solver{ nullptr };
    std::unique_ptr<btDiscreteDynamicsWorld> s_dynamicsWorld{ nullptr };

    std::unordered_map<uintptr_t, Glib::WeakPtr<Glib::Internal::Interface::IRigidbody>> s_rigidbodys;
    std::deque<std::pair<GameObjectPtr, GameObjectPtr>> s_collisionEnterCallbacks;
    std::deque<std::pair<GameObjectPtr, GameObjectPtr>> s_collisionStayCallbacks;
    std::deque<std::pair<GameObjectPtr, GameObjectPtr>> s_collisionExitCallbacks;
}

namespace
{
    auto s_componentManager = Glib::Internal::ComponentManager::Instance();
}

bool Glib::Internal::Physics::BulletPhysicsManager::Initialize()
{
    // bulletの各オブジェクトの初期化
    s_broadphase = std::make_unique<btDbvtBroadphase>();
    if (s_broadphase == nullptr) return false;

    s_configuration = std::make_unique<btDefaultCollisionConfiguration>();
    if (s_configuration == nullptr) return false;

    s_dispatcher = std::make_unique<btCollisionDispatcher>(s_configuration.get());
    if (s_dispatcher == nullptr) return false;

    s_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    if (s_solver == nullptr) return false;

    s_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        s_dispatcher.get(),
        s_broadphase.get(),
        s_solver.get(),
        s_configuration.get()
    );
    if (s_dynamicsWorld == nullptr) return false;

    // 重力の設定
    s_dynamicsWorld->setGravity(TobtVector3(DEFAULT_GRAVITY));
    return true;
}

void Glib::Internal::Physics::BulletPhysicsManager::Update()
{
    for (const auto& [ID, rigidbody] : s_rigidbodys)
    {
        rigidbody->SyncToBullet();
    }

    s_dynamicsWorld->stepSimulation(GameTimer::DeltaTime(), SIMULATE_SUB_STEP, GameTimer::FixedDeltaTime());

    int numManifolds = s_dispatcher->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold = s_dispatcher->getManifoldByIndexInternal(i);
        const btCollisionObject* objA = contactManifold->getBody0();
        const btCollisionObject* objB = contactManifold->getBody1();
        int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++)
        {
            auto& point = contactManifold->getContactPoint(j);
            if (point.getDistance() < 0.0f)
            {
                const auto& pointA = point.getPositionWorldOnA();
                const auto& pointB = point.getPositionWorldOnB();
                const auto& normal = point.m_normalWorldOnB;

            }
        }
    }

    for (const auto& [ID, rigidbody] : s_rigidbodys)
    {
        rigidbody->SyncFromBullet();
    }

    // 衝突判定のコールバックを呼び出す
    ExecuteTriggerCallbacks();
    ExecuteCollisionCallbacks();
}


void Glib::Internal::Physics::BulletPhysicsManager::ExecuteTriggerCallbacks()
{

}

void Glib::Internal::Physics::BulletPhysicsManager::ExecuteCollisionCallbacks()
{

}

bool Glib::Internal::Physics::BulletPhysicsManager::Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* hit)
{
    auto from = TobtVector3(origin);
    auto to = TobtVector3(origin + direction * maxDistance);
    btCollisionWorld::ClosestRayResultCallback result{ TobtVector3(origin), TobtVector3(direction * maxDistance) };
    s_dynamicsWorld->rayTest(TobtVector3(origin), TobtVector3(direction * maxDistance), result);
    return result.hasHit();
}

bool Glib::Internal::Physics::BulletPhysicsManager::RaycastAll(const Vector3& origin, const Vector3& direction, float maxDistance, std::vector<RaycastHit>* hit)
{
    return false;
}

bool Glib::Internal::Physics::BulletPhysicsManager::AddRigidbody(const WeakPtr<Glib::Internal::Interface::IRigidbody>& rigidbody)
{
    if (rigidbody.expired()) return false;
    s_dynamicsWorld->addRigidBody(rigidbody->GetbtRigidbody());
    s_rigidbodys.emplace(rigidbody.getId(), rigidbody);
    return true;
}

btVector3 Glib::Internal::Physics::BulletPhysicsManager::TobtVector3(const Vector3& v)
{
    return btVector3{ v.x, v.y, v.z };
}
