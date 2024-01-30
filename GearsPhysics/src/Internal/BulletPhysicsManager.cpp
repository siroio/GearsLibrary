#include <Internal/BulletPhysicsManager.h>
#include <btBulletDynamicsCommon.h>
#include <Vector3.h>
#include <GameTimer.h>
#include <memory>

namespace
{
    Vector3 DEFAULT_GRAVITY{ 0, -9.81f, 0 };
    unsigned int SIMULATE_SUB_STEP{ 10 };
    std::unique_ptr<btBroadphaseInterface> s_broadphase{ nullptr };
    std::unique_ptr<btDefaultCollisionConfiguration> s_configuration{ nullptr };
    std::unique_ptr<btCollisionDispatcher> s_dispatcher{ nullptr };
    std::unique_ptr<btSequentialImpulseConstraintSolver> s_solver{ nullptr };
    std::unique_ptr<btDiscreteDynamicsWorld> s_dynamicsWorld{ nullptr };
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
    s_dynamicsWorld->stepSimulation(GameTimer::DeltaTime(), SIMULATE_SUB_STEP, GameTimer::FixedDeltaTime());
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

bool Glib::Internal::Physics::BulletPhysicsManager::AddRigidbody(btRigidBody* rigidbody)
{
    if (rigidbody == nullptr) return false;
    s_dynamicsWorld->addRigidBody(rigidbody);
    return true;
}

btVector3 Glib::Internal::Physics::BulletPhysicsManager::TobtVector3(const Vector3& v)
{
    return btVector3{ v.x, v.y, v.z };
}
