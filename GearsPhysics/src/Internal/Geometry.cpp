#include <Internal/Geometry.h>
#include <Internal/PhysXManager.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <Mathf.h>
#include <Mesh.h>
#include <Debugger.h>

namespace
{
    auto s_physX = Glib::Internal::Physics::PhysXManager::Instance();
}

physx::PxBoxGeometry Glib::Internal::Geometory::CreateBox(const GameObjectPtr& gameObject, const Vector3& size)
{
    physx::PxBoxGeometry geometry{};
    const Vector3& scale = gameObject->Transform()->Scale();
    geometry.halfExtents.x = size.x * Mathf::Max(scale.x * 0.5f, Mathf::EPSILON);
    geometry.halfExtents.y = size.y * Mathf::Max(scale.y * 0.5f, Mathf::EPSILON);
    geometry.halfExtents.z = size.z * Mathf::Max(scale.z * 0.5f, Mathf::EPSILON);
    return geometry;
}

physx::PxCapsuleGeometry Glib::Internal::Geometory::CreateCapsule(const GameObjectPtr& gameObject, float height, float radius)
{
    const Vector3& scale = gameObject->Transform()->Scale();
    float maxScale = Mathf::Max(scale.x, scale.z);

    radius *= maxScale;
    float halfHeight = (height * scale.y * 0.5f) - radius;
    physx::PxCapsuleGeometry geometry{ radius, Mathf::Max(height, Mathf::EPSILON) };
    return geometry;
}

physx::PxTriangleMesh* Glib::Internal::Geometory::CreateTriangleMesh(const GameObjectPtr& gameObject, const Mesh& mesh)
{
    physx::PxTolerancesScale tolerancesScale;
    physx::PxCookingParams cookingParams{ tolerancesScale };
    cookingParams.convexMeshCookingType = physx::PxConvexMeshCookingType::Enum::eQUICKHULL;
    cookingParams.gaussMapLimit = 256;

    const auto& vertices = mesh.Vertices();
    const auto& indices = mesh.Indices();
    physx::PxTriangleMeshDesc pxMeshDesc{};
    pxMeshDesc.setToDefault();
    pxMeshDesc.points.count = static_cast<physx::PxU32>(vertices.size());
    pxMeshDesc.points.stride = sizeof(physx::PxU32);
    pxMeshDesc.points.data = vertices.data();
    pxMeshDesc.triangles.count = static_cast<physx::PxU32>(indices.size());
    pxMeshDesc.triangles.stride = sizeof(physx::PxU32);
    pxMeshDesc.triangles.data = indices.data();
    pxMeshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

    physx::PxDefaultMemoryOutputStream writeBuffer;
    if (!PxCookTriangleMesh(cookingParams, pxMeshDesc, writeBuffer))
    {
        Debug::Log("PxCookTriangleMesh failed.", LogLevel::Error);
    }
    physx::PxDefaultMemoryInputData readBuffer{ writeBuffer.getData(), writeBuffer.getSize() };

    return s_physX->Physcs().createTriangleMesh(readBuffer);
}

physx::PxConvexMesh* Glib::Internal::Geometory::CreateConvexMesh(const GameObjectPtr& gameObject, const Mesh& mesh)
{
    physx::PxTolerancesScale tolerancesScale;
    physx::PxCookingParams cookingParams{ tolerancesScale };
    cookingParams.convexMeshCookingType = physx::PxConvexMeshCookingType::Enum::eQUICKHULL;
    cookingParams.gaussMapLimit = 256;

    const auto& vertices = mesh.Vertices();
    physx::PxConvexMeshDesc pxMeshDesc{};
    pxMeshDesc.setToDefault();
    pxMeshDesc.points.count = static_cast<physx::PxU32>(vertices.size());
    pxMeshDesc.points.stride = sizeof(physx::PxU32);
    pxMeshDesc.points.data = vertices.data();
    pxMeshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxDefaultMemoryOutputStream writeBuffer;
    if (!PxCookConvexMesh(cookingParams, pxMeshDesc, writeBuffer))
    {
        Debug::Log("PxCookTriangleMesh failed.", LogLevel::Error);
    }
    physx::PxDefaultMemoryInputData readBuffer{ writeBuffer.getData(), writeBuffer.getSize() };

    return s_physX->Physcs().createConvexMesh(readBuffer);
}
