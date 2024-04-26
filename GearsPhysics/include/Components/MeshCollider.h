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
        ~MeshCollider();
        void Start();

        unsigned int MeshID() const;
        void MeshID(unsigned int id);

        bool Convex() const;
        void Convex(bool convex);

        bool FlipNormals() const;
        void FlipNormals(bool flip);

    private:
        void SyncGeometry() override;
        void CreateMesh();

    private:
        PxMeshPtr meshPtr_;
        unsigned int meshID_{ UINT_MAX };
        bool enableMesh_{ false };
        bool isConvex_{ false };
        bool isFlipNormals_{ false };
        bool initialized{ false };
    };
}
