#pragma once
#include <GameObjectPtr.h>
#include <Vector3.h>

namespace physx
{
    class PxBoxGeometry;
    class PxCapsuleGeometry;
    class PxTriangleMesh;
    class PxConvexMesh;
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
        static physx::PxTriangleMesh* CreateTriangleMesh(const GameObjectPtr& gameObject, const Mesh& mesh);
        static physx::PxConvexMesh* CreateConvexMesh(const GameObjectPtr& gameObject, const Mesh& mesh);
    };
}
