#pragma once
#include <Component.h>
#include <BitFlag.h>
#include <Internal/IRigidbody.h>

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

        float Mass() const;
        void Mass(float mass);

        bool UseGravity() const;
        void UseGravity(bool useGravity);

        void OnGUI() override;

    private:
        const GameObjectPtr& GetGameObject() override;
        void SyncToBullet() override;
        void SyncFromBullet() override;

    private:
        bool useGravity_{ true };
        float mass_{ 1.0f };
        BitFlag<RigidbodyConstraints> constrants_;
    };
}
