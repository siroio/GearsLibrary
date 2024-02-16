#pragma once
#include <Components/Collider.h>
#include <PxPhysicsAPI.h>
#include <variant>

namespace Glib
{
    class MeshCollider : public Collider
    {
        using PxMeshPtr = std::variant<physx::PxTriangleMesh*, physx::PxConvexMesh*>;

    public:
        void Start();

    private:
        void SyncGeometry() override;

    private:
        PxMeshPtr meshPtr_;
        bool isConvex{ false };
    };
}
