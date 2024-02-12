#pragma once

namespace Glib::Internal::Interface
{
    class ICollider
    {
    protected:
        ICollider() = default;
        virtual ~ICollider() = default;

    public:
        virtual void SyncGeometry() = 0;
        virtual void SyncActive() = 0;
    };
}
