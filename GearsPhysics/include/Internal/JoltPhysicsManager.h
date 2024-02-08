#pragma once
#include <Singleton.h>
#include <Internal/ISystem.h>
#include <vector>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyInterface.h>

struct Matrix4x4;
struct Vector3;
struct Quaternion;

namespace Glib
{
    struct RaycastHit;
}

namespace JPH
{
    class Body;
    class BodyCreationSettings;
}

namespace Glib::Internal::Interface
{
    class IRigidbody;
}

// TEST Layer
namespace Layers
{
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

namespace BroadPhaseLayers
{
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr JPH::uint NUM_LAYERS(2);
};

namespace Glib::Internal::Physics
{
    class JoltPhysicsManager :
        public Interface::ISystem,
        public SingletonPtr<JoltPhysicsManager>,
        public JPH::ObjectLayerPairFilter,
        public JPH::BroadPhaseLayerInterface,
        public JPH::ObjectVsBroadPhaseLayerFilter,
        public JPH::ContactListener,
        public JPH::BodyActivationListener
    {
        JoltPhysicsManager() = default;
        friend WeakPtr<JoltPhysicsManager> SingletonPtr<JoltPhysicsManager>::Instance();

    public:
        bool Initialize();
        void Finalize();
        void Update();

        void ExecuteTriggerCallbacks();
        void ExecuteCollisionCallbacks();

        bool Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* hit);
        bool RaycastAll(const Vector3& origin, const Vector3& direction, float maxDistance, std::vector<RaycastHit>* hit);

        bool AddRigidbody(const WeakPtr<Interface::IRigidbody>& rigidbody);
        JPH::Body* CreateBody(const JPH::BodyCreationSettings& bodySettings);
        void RemoveRigidbody(const WeakPtr<Interface::IRigidbody>& rigidbody);
        JPH::BodyInterface& GetBodyInterface() const;

    public:
        static Matrix4x4 ToMatrix4x4(const JPH::RMat44& mat);
        static JPH::RVec3 ToRVec3(const Vector3& v);
        static JPH::Quat ToQuat(const Quaternion& qua);

    private:
        JPH::uint GetNumBroadPhaseLayers() const override;
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
        const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
        virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const;
        virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
        virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult);
        virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings);
        virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings);
        virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair);
        void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
        void OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
    };
}
