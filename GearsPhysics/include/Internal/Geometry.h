#pragma once
#include <GameObjectPtr.h>
#include <Vector3.h>

namespace physx
{
    class PxBoxGeometry;
    class PxCapsuleGeometry;
    class PxTriangleMesh;
    class PxConvexMesh;
    class PxTriangleMeshGeometry;
    class PxConvexMeshGeometry;
}

namespace Glib
{
    class Mesh;
}

namespace Glib::Internal
{
    class Geometory
    {
    private:
        Geometory() = default;
        ~Geometory() = default;

    public:
        static physx::PxBoxGeometry CreateBox(const GameObjectPtr& gameObject, const Vector3& size);
        static physx::PxCapsuleGeometry CreateCapsule(const GameObjectPtr& gameObject, float height, float radius);
        static physx::PxTriangleMesh* CreateTriangleMesh(const Mesh& mesh);
        static physx::PxConvexMesh* CreateConvexMesh(const Mesh& mesh);
        static physx::PxTriangleMeshGeometry CreateTriangleMesh(const GameObjectPtr& gameObject, physx::PxTriangleMesh* const mesh);
        static physx::PxConvexMeshGeometry CreateConvexMesh(const GameObjectPtr& gameObject, physx::PxConvexMesh* const mesh);
    };
}
