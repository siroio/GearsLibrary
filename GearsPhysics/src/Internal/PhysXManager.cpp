#include <Internal/PhysXManager.h>
#include <Internal/PhysicsDebugRenderer.h>
#include <Internal/ComponentManager.h>
#include <Internal/IRigidbody.h>
#include <Internal/ICollider.h>
#include <InlineUtility.h>
#include <Vector3.h>
#include <Color.h>
#include <Quaternion.h>
#include <Matrix4x4.h>
#include <GameTimer.h>
#include <GameObject.h>
#include <RaycastHit.h>
#include <Debugger.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <list>
#include <Layer2D.h>
#include <ranges>
#include <Mathf.h>

namespace
{
    float s_accumulatedTime{ 0.0f };
    using GameObjectPair = std::pair<GameObjectPtr, GameObjectPtr>;

    const Vector3 DEFAULT_GRAVITY{ 0, -9.81f, 0 };

    Glib::Layer2D s_layer;
    std::unordered_map<uintptr_t, Glib::WeakPtr<Glib::Internal::Interface::IRigidbody>> s_rigidbodys;
    std::unordered_map<uintptr_t, GameObjectPtr> s_staticRigidbodys;
    std::list<Glib::WeakPtr<Glib::Internal::Interface::ICollider>> s_colliders;

    std::list<GameObjectPair> s_triggerEnterCallbacks;
    std::list<GameObjectPair> s_triggerStayCallbacks;
    std::list<GameObjectPair> s_triggerExitCallbacks;

    std::list<GameObjectPair> s_collisionEnterCallbacks;
    std::list<GameObjectPair> s_collisionStayCallbacks;
    std::list<GameObjectPair> s_collisionExitCallbacks;

    physx::PxDefaultAllocator s_defaultAllocator;
    physx::PxDefaultErrorCallback s_defaultErrorCallback;
    physx::PxFoundation* s_foundation{ nullptr };
    physx::PxPhysics* s_physics{ nullptr };
    physx::PxDefaultCpuDispatcher* s_dispatcher{ nullptr };
    physx::PxScene* s_scene{ nullptr };
}

namespace
{
    bool IsDeleteCollider(const Glib::WeakPtr<Glib::Internal::Interface::ICollider>& collider)
    {
        return collider.expired();
    }

    physx::PxFilterFlags SimulationFilterShader(
        physx::PxFilterObjectAttributes attributes0,
        physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1,
        physx::PxFilterData filterData1,
        physx::PxPairFlags& pairFlags,
        const void* constantBlock,
        physx::PxU32 constantBlockSize)
    {
        // 衝突が無効なグループであれば衝突を無視
        if (!s_layer.Validate(filterData0.word0, filterData1.word0))
        {
            return physx::PxFilterFlag::eSUPPRESS;
        }

        // トリガーかどうか
        const bool isTrigger = physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1);
        if (isTrigger)
        {
            pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;
            return physx::PxFilterFlag::eDEFAULT;
        }

        pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;
        pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
        pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;

        return physx::PxFilterFlag::eDEFAULT;
    }

    GameObjectPtr GetGameObject(const physx::PxActor* actor)
    {
        uintptr_t id = reinterpret_cast<uintptr_t>(actor);
        auto rb = s_rigidbodys.find(id);
        if (rb != s_rigidbodys.end())
        {
            return rb->second->GetGameObject();
        }
        auto staticRb = s_staticRigidbodys.find(id);
        if (staticRb != s_staticRigidbodys.end())
        {
            return staticRb->second;
        }
        return nullptr;
    }
}

namespace
{
    auto s_componentManager = Glib::Internal::ComponentManager::Instance();
#if defined(DEBUG) || defined(_DEBUG)
    auto s_debugRenderer = Glib::Internal::Physics::PhysicsDebugRenderer::Instance();
#endif
}

bool Glib::Internal::Physics::PhysXManager::Initialize()
{
    s_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_defaultAllocator, s_defaultErrorCallback);
    if (s_foundation == nullptr) return false;

    const auto scale = physx::PxTolerancesScale{};
    s_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *s_foundation, scale, true);
    if (s_physics == nullptr) return false;

    s_dispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency() - 1);
    if (s_dispatcher == nullptr) return false;

    physx::PxSceneDesc sceneDesc{ s_physics->getTolerancesScale() };
    sceneDesc.gravity = ToPxVec3(DEFAULT_GRAVITY);
    sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
    sceneDesc.filterShader = SimulationFilterShader;
    sceneDesc.cpuDispatcher = s_dispatcher;
    sceneDesc.simulationEventCallback = static_cast<physx::PxSimulationEventCallback*>(this);
    s_scene = s_physics->createScene(sceneDesc);
    if (s_scene == nullptr) return false;

#if defined(DEBUG) || defined(_DEBUG)
    s_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
    s_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
#endif

    return true;
}

void Glib::Internal::Physics::PhysXManager::Finalize()
{
    if (s_scene != nullptr)
    {
        s_scene->release();
        s_scene = nullptr;
    }
    if (s_dispatcher != nullptr)
    {
        s_dispatcher->release();
        s_dispatcher = nullptr;
    }
    if (s_physics != nullptr)
    {
        s_physics->release();
        s_physics = nullptr;
    }
    if (s_foundation != nullptr)
    {
        s_foundation->release();
        s_foundation = nullptr;
    }
}

void Glib::Internal::Physics::PhysXManager::Update()
{
    // rigidbodyのmapからvalueだけ参照
    const auto& rigidbodys = s_rigidbodys | std::ranges::views::values;

    for (const auto& rigidbody : rigidbodys)
    {
        rigidbody->SyncToPhysics();
    }

    s_colliders.remove_if(IsDeleteCollider);
    for (const auto& collider : s_colliders)
    {
        collider->SyncActive();
        collider->SyncGeometry();
    }


    s_accumulatedTime = Mathf::Min(s_accumulatedTime + GameTimer::DeltaTime(), GameTimer::MaximumAllowedTimeStep());

    // fixedUpdateを溜まった時間分進める
    while (s_accumulatedTime >= GameTimer::FixedTimeStep())
    {
        if (GameTimer::FixedDeltaTime() > Mathf::EPSILON)
        {
            s_scene->simulate(GameTimer::FixedDeltaTime());
            s_scene->fetchResults(true);
        }
        s_accumulatedTime -= GameTimer::FixedTimeStep();
    }

    for (const auto& rigidbody : rigidbodys)
    {
        rigidbody->SyncFromPhysics();
    }

    // 衝突判定のコールバックを呼び出す
    ExecuteTriggerCallbacks();
    ExecuteCollisionCallbacks();

#if defined(DEBUG) || defined(_DEBUG)
    const auto& renderBuffer = s_scene->getRenderBuffer();
    const auto* line = renderBuffer.getLines();
    for (physx::PxU32 i = 0; i < renderBuffer.getNbLines(); i++)
    {
        s_debugRenderer->AddVertex(ToVector3(line[i].pos0), Color::Green());
        s_debugRenderer->AddVertex(ToVector3(line[i].pos1), Color::Green());
    }
#endif
}

void Glib::Internal::Physics::PhysXManager::ExecuteTriggerCallbacks()
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
            ComponentFunctionType::OnTriggerExit,
            body2
        );
    }

    s_triggerEnterCallbacks.clear();
    s_triggerExitCallbacks.clear();
}

void Glib::Internal::Physics::PhysXManager::ExecuteCollisionCallbacks()
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

bool Glib::Internal::Physics::PhysXManager::Raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hit, float maxDistance)
{
    physx::PxRaycastBuffer buffer{};
    const auto& isHit = s_scene->raycast(ToPxVec3(origin), ToPxVec3(direction), maxDistance, buffer);

    if (!isHit) return false;
    const auto& gameObject = GetGameObject(buffer.block.actor);

    if (gameObject.expired()) return false;
    hit.gameObject = gameObject;
    hit.distance = buffer.block.distance;
    hit.normal = ToVector3(buffer.block.normal);
    hit.point = ToVector3(buffer.block.position);
    return isHit;
}

bool Glib::Internal::Physics::PhysXManager::RaycastAll(const Vector3& origin, const Vector3& direction, std::vector<RaycastHit>& hits, float maxDistance)
{
    physx::PxRaycastBuffer buffer{};
    buffer.maxNbTouches = s_physics->getNbShapes();
    std::vector<physx::PxRaycastHit> raycastHits{};
    raycastHits.resize(buffer.maxNbTouches);
    buffer.touches = raycastHits.data();

    bool isHit = s_scene->raycast(ToPxVec3(origin), ToPxVec3(direction), maxDistance, buffer);
    if (!isHit) return false;

    hits.resize(buffer.nbTouches);

    for (physx::PxU32 i = 0; i < buffer.nbTouches; i++)
    {
        const physx::PxRaycastHit& hitInfo = buffer.getTouch(i);
        const auto& gameObject = GetGameObject(hitInfo.actor);
        if (gameObject.expired()) continue;
        hits.at(i).gameObject = gameObject;
        hits.at(i).distance = hitInfo.distance;
        hits.at(i).normal = ToVector3(hitInfo.normal);
        hits.at(i).point = ToVector3(hitInfo.position);
    }

    return isHit;
}

physx::PxRigidActor* Glib::Internal::Physics::PhysXManager::CreateRigidBody(const Vector3& position, const Quaternion& rotation, const WeakPtr<Interface::IRigidbody>& rigidbody)
{
    physx::PxTransform pose{ ToPxVec3(position), ToPxQuat(rotation) };
    physx::PxRigidActor* actor = s_physics->createRigidDynamic(pose);
    s_rigidbodys.emplace(reinterpret_cast<uintptr_t>(actor), rigidbody);
    s_scene->addActor(*actor);
    return actor;
}

physx::PxRigidActor* Glib::Internal::Physics::PhysXManager::CreateRigidStatic(const Vector3& position, const Quaternion& rotation, const GameObjectPtr& gameObject)
{
    physx::PxTransform pose{ ToPxVec3(position), ToPxQuat(rotation) };
    physx::PxRigidActor* actor = s_physics->createRigidStatic(pose);
    s_staticRigidbodys.emplace(reinterpret_cast<uintptr_t>(actor), gameObject);
    s_scene->addActor(*actor);
    return actor;
}

void Glib::Internal::Physics::PhysXManager::RemoveActor(physx::PxActor* actor)
{
    if (actor == nullptr) return;
    uintptr_t actorID = reinterpret_cast<uintptr_t>(actor);
    s_rigidbodys.erase(actorID);
    s_staticRigidbodys.erase(actorID);
    s_scene->removeActor(*actor);
    actor->release();
}

physx::PxMaterial* Glib::Internal::Physics::PhysXManager::CreateMaterial(float dynamicFriction, float staticFriction, float bounce)
{
    return s_physics->createMaterial(staticFriction, dynamicFriction, bounce);
}

void Glib::Internal::Physics::PhysXManager::AddCollider(const WeakPtr<Glib::Internal::Interface::ICollider>& collider)
{
    s_colliders.push_back(collider);
}

void Glib::Internal::Physics::PhysXManager::SetCollisionFlags(unsigned int layer1, unsigned int layer2, bool enable)
{
    s_layer.Set(layer1, layer2, enable);
}

physx::PxPhysics& Glib::Internal::Physics::PhysXManager::Physcs()
{
    return *s_physics;
}

physx::PxVec3 Glib::Internal::Physics::PhysXManager::ToPxVec3(const Vector3& v)
{
    return physx::PxVec3{ v.x, v.y, v.z };
}

Vector3 Glib::Internal::Physics::PhysXManager::ToVector3(const physx::PxVec3& v)
{
    return Vector3{ v.x, v.y, v.z };
}

physx::PxQuat Glib::Internal::Physics::PhysXManager::ToPxQuat(const Quaternion& q)
{
    return physx::PxQuat{ q.x, q.y, q.z, q.w };
}

Quaternion Glib::Internal::Physics::PhysXManager::ToQuaternion(const physx::PxQuat& q)
{
    return Quaternion{ q.x, q.y, q.z, q.w };
}

void Glib::Internal::Physics::PhysXManager::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    for (physx::PxU32 i = 0; i < nbPairs; i++)
    {
        const physx::PxU16& event = pairs[i].events;
        const auto& gameObject0 = GetGameObject(pairHeader.actors[0]);
        const auto& gameObject1 = GetGameObject(pairHeader.actors[1]);
        if (gameObject0.expired() || gameObject1.expired()) return;

        if ((event & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) != 0)
        {
            // 衝突時
            s_collisionEnterCallbacks.push_back({ gameObject0, gameObject1 });
            s_collisionEnterCallbacks.push_back({ gameObject1, gameObject0 });
        }
        else if ((event & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) != 0)
        {
            // 離散時
            s_collisionExitCallbacks.push_back({ gameObject0, gameObject1 });
            s_collisionExitCallbacks.push_back({ gameObject1, gameObject0 });
            s_collisionStayCallbacks.remove({ gameObject0, gameObject1 });
            s_collisionStayCallbacks.remove({ gameObject1, gameObject0 });
        }
    }
}

void Glib::Internal::Physics::PhysXManager::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for (physx::PxU32 i = 0; i < count; i++)
    {
        const physx::PxU16& event = pairs[i].status;
        const auto& triggerObject = GetGameObject(pairs[i].triggerActor);
        const auto& otherObject = GetGameObject(pairs[i].otherActor);
        if (triggerObject.expired() || otherObject.expired()) return;

        if ((event & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) != 0)
        {
            // 衝突時
            s_triggerEnterCallbacks.push_back({ triggerObject, otherObject });
            s_triggerEnterCallbacks.push_back({ otherObject, triggerObject });
        }
        else if ((event & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) != 0)
        {
            // 離散時
            s_triggerExitCallbacks.push_back({ triggerObject, otherObject });
            s_triggerExitCallbacks.push_back({ otherObject, triggerObject });
            s_triggerStayCallbacks.remove({ triggerObject, otherObject });
            s_triggerStayCallbacks.remove({ otherObject, triggerObject });
        }
    }
}

void Glib::Internal::Physics::PhysXManager::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{}

void Glib::Internal::Physics::PhysXManager::onWake(physx::PxActor** actors, physx::PxU32 count)
{}

void Glib::Internal::Physics::PhysXManager::onSleep(physx::PxActor** actors, physx::PxU32 count)
{}

void Glib::Internal::Physics::PhysXManager::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{}
