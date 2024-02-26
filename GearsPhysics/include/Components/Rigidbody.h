#pragma once
#include <Component.h>
#include <BitFlag.h>
#include <Internal/IRigidbody.h>
#include <Vector3.h>

namespace Glib
{
    enum class RigidbodyConstraints
    {
        None = 0,
        FreezePositionX = 1 << 0,
        FreezePositionY = 1 << 1,
        FreezePositionZ = 1 << 2,
        FreezeRotationX = 1 << 3,
        FreezeRotationY = 1 << 4,
        FreezeRotationZ = 1 << 5,

        FreezePosition = FreezePositionX | FreezePositionY | FreezePositionZ,
        FreezeRotation = FreezeRotationX | FreezeRotationY | FreezeRotationZ,
        FreezeAll = FreezePosition | FreezeRotation,
    };

    enum class ForceMode
    {
        Force = 0,
        Acceleration = 3,
        Impulse = 1,
        VelocityChange = 2
    };

    class Rigidbody :
        public Component,
        public Internal::Interface::IRigidbody
    {
    public:
        void Start();
        void OnDestroy();

        void AddForce(const Vector3& force, ForceMode mode = ForceMode::Force);
        void AddTorque(const Vector3& torque, ForceMode mode = ForceMode::Force);

        const BitFlag<RigidbodyConstraints>& Constraints() const;
        void Constraints(const RigidbodyConstraints& constraints);

        bool IsKinematic() const;
        void IsKinematic(bool enable);

        float Mass() const;
        void Mass(float mass);

        Vector3 LinearVelocity() const;
        void LinearVelocity(const Vector3& linearVelocity);

        Vector3 AngularVelocity() const;
        void AngularVelocity(const Vector3& angularVelocity);

        float LinearDamping() const;
        void LinearDamping(float linearDamping);

        float AngularDamping() const;
        void AngularDamping(float angularDamping);

        bool UseGravity() const;
        void UseGravity(bool useGravity);

        void OnGUI() override;

    private:
        const GameObjectPtr& GetGameObject() override;
        physx::PxRigidDynamic& GetRigidDynamic() override;
        void SyncToPhysics() override;
        void SyncFromPhysics() override;

    private:
        physx::PxRigidDynamic* rigidDynamic_{ nullptr };
        bool useGravity_{ true };
        bool isKinematic_{ false };
        float mass_{ 1.0f };
        float linearDamping_{ 0.0f };
        float angularDamping_{ 0.05f };
        Vector3 linearVelocity_{ 0.0f, 0.0f, 0.0f };
        Vector3 angularVelocity_{ 0.0f, 0.0f, 0.0f };
        BitFlag<RigidbodyConstraints> constrants_;
    };
}
