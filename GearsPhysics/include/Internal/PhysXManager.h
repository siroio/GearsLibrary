#pragma once
#include <Singleton.h>
#include <Internal/ISystem.h>
#include <vector>
#include <PxConfig.h>
#include <PxPhysicsAPI.h>

struct Matrix4x4;
struct Vector3;
struct Quaternion;
struct Color;

namespace Glib
{
    struct RaycastHit;
}

namespace JPH
{
    class Body;
    class BodyCreationSettings;
    class Color;
}

namespace Glib::Internal::Interface
{
    class IRigidbody;
    class ICollider;
}

namespace Glib::Internal::Physics
{
    class PhysXManager :
        public Interface::ISystem,
        public SingletonPtr<PhysXManager>,
        public physx::PxSimulationEventCallback
    {
        PhysXManager() = default;
        friend WeakPtr<PhysXManager> SingletonPtr<PhysXManager>::Instance();

    public:
        bool Initialize();
        void Finalize();
        void Update();

        void ExecuteTriggerCallbacks();
        void ExecuteCollisionCallbacks();

        bool Raycast(const Vector3& origin, const Vector3& direction, RaycastHit* hit, float maxDistance = FLT_MAX);
        bool RaycastAll(const Vector3& origin, const Vector3& direction, std::vector<RaycastHit>* hit, float maxDistance = FLT_MAX);

        physx::PxRigidDynamic* CreateRigidBody(const Vector3& position, const Quaternion& rotation, const WeakPtr<Interface::IRigidbody>& rigidbody);
        void RemoveRigidbody(const WeakPtr<Interface::IRigidbody>& rigidbody);
        physx::PxMaterial* CreateMaterial(float dynamicFriction, float staticFriction, float bounce);
        void AddCollider(const WeakPtr<Interface::ICollider>& collider);

    private:
        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
        void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
        void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
        void onWake(physx::PxActor** actors, physx::PxU32 count) override;
        void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
        void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

    public:
        static physx::PxVec3 ToPxVec3(const Vector3& v);
        static Vector3 ToVector3(const physx::PxVec3& v);
        static physx::PxQuat ToPxQuat(const Quaternion& q);
        static Quaternion ToQuaternion(const physx::PxQuat& q);
    };
}
