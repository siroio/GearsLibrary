#pragma once
#include <GameObjectPtr.h>

namespace physx
{
    class PxRigidDynamic;
}

namespace Glib::Internal::Interface
{
    class IRigidbody
    {
    protected:
        IRigidbody() = default;
        virtual ~IRigidbody() = default;

    public:
        virtual const GameObjectPtr& GetGameObject() = 0;
        virtual physx::PxRigidDynamic& GetRigidDynamic() = 0;
        virtual void SyncToPhysics() = 0;
        virtual void SyncFromPhysics() = 0;
    };
}
