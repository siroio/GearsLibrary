#pragma once
#include <Singleton.h>
#include <Internal/ISystem.h>
#include <vector>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

struct Vector3;

namespace Glib
{
    struct RaycastHit;
}

namespace Glib::Internal::Interface
{
    class IRigidbody;
}

namespace Glib::Internal::Physics
{
    class JoltPhysicsManager :
        public Interface::ISystem,
        public SingletonPtr<JoltPhysicsManager>,
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

    private:
        JPH::uint GetNumBroadPhaseLayers() const override;
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
        const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
        virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
        virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult);
        virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings);
        virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings);
        virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair);
        void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
        void OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
    };
}
