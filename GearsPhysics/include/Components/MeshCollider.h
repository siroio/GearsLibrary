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

        unsigned int MeshID() const;
        void MeshID(unsigned int id);

    private:
        void SyncGeometry() override;
        void CreateMesh();

    private:
        PxMeshPtr meshPtr_;
        unsigned int meshID_{ UINT_MAX };
        bool enableMesh_{ false };
        bool isConvex_{ false };
    };
}
