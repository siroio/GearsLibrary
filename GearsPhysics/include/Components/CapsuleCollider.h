#pragma once
#include <Components/Collider.h>

namespace Glib
{
    class CapsuleCollider : public Collider
    {
        // Collider を介して継承されました
        void SyncGeometry() override;
    };
}
