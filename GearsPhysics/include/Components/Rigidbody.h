#pragma once
#include <Component.h>
#include <BitFlag.h>
#include <Internal/IRigidbody.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

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
        void OnDestroy();
        float Mass() const;
        void Mass(float mass);

        bool UseGravity() const;
        void UseGravity(bool useGravity);

        void OnGUI() override;

    private:
        const GameObjectPtr& GetGameObject() override;
        const JPH::BodyID& GetBodyID() override;
        void SyncToPhysics() override;
        void SyncFromPhysics() override;

    private:
        bool useGravity_{ true };
        float mass_{ 1.0f };
        JPH::Body* body_;
        BitFlag<RigidbodyConstraints> constrants_;
    };
}
