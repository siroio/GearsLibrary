#pragma once
#include <Component.h>
#include <BitFlag.h>
#include <Internal/IRigidbody.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btDefaultMotionState.h>
class btVector3;
class btQuaternion;
struct Vector3;
struct Quaternion;

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

    class Rigidbody :
        public Component,
        public Internal::Interface::IRigidbody
    {
    public:
        void Start();
        void Update();

        float Mass() const;
        void Mass(float mass);

        bool UseGravity() const;
        void UseGravity(bool useGravity);

        void OnGUI() override;

        const GameObjectPtr& GetGameObject() override;
        void SyncTransform() override;

    private:
        static btVector3 ToVec3(const Vector3& v);
        static btQuaternion ToQuat(const Quaternion& v);
        static Vector3 FromVec3(const btVector3& v);
        static Quaternion FromQuat(const btQuaternion& v);

    private:
        std::unique_ptr<btRigidBody> rigidbody_;
        std::unique_ptr<btDefaultMotionState> motionState_;
        std::unique_ptr<btCollisionShape> shape_;
        bool useGravity_{ true };
        float mass_{ 1.0f };
        BitFlag<RigidbodyConstraints> constrants_;
    };
}
