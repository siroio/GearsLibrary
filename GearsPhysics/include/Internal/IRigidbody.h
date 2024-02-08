#pragma once
#include <GameObjectPtr.h>

namespace Glib::Internal::Interface
{
    class IRigidbody
    {
    protected:
        IRigidbody() = default;
        virtual ~IRigidbody() = default;

    public:
        virtual const GameObjectPtr& GetGameObject() = 0;
        virtual void SyncToBullet() = 0;
        virtual void SyncFromBullet() = 0;
    };
}
