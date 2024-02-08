#pragma once
#include <GameObjectPtr.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace Glib::Internal::Interface
{
    class IRigidbody
    {
    protected:
        IRigidbody() = default;
        virtual ~IRigidbody() = default;

    public:
        virtual const GameObjectPtr& GetGameObject() = 0;
        virtual const JPH::BodyID& GetBodyID() = 0;
        virtual void SyncToPhysics() = 0;
        virtual void SyncFromPhysics() = 0;
    };
}
