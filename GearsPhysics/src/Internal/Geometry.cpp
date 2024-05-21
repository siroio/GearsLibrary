#include <Internal/Geometry.h>
#include <Internal/PhysXManager.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <Mathf.h>
#include <Mesh.h>
#include <Debugger.h>
#include <algorithm>
#include <ranges>

namespace
{
    auto s_physX = Glib::Internal::Physics::PhysXManager::Instance();
}

physx::PxBoxGeometry Glib::Internal::Geometry::CreateBox(const GameObjectPtr& gameObject, const Vector3& size)
{
    physx::PxBoxGeometry geometry{};
    const Vector3& scale = gameObject->Transform()->Scale();
    geometry.halfExtents.x = size.x * Mathf::Max(scale.x * 0.5f, Mathf::EPSILON);
    geometry.halfExtents.y = size.y * Mathf::Max(scale.y * 0.5f, Mathf::EPSILON);
    geometry.halfExtents.z = size.z * Mathf::Max(scale.z * 0.5f, Mathf::EPSILON);
    return geometry;
}

physx::PxSphereGeometry Glib::Internal::Geometry::CreateSphere(const GameObjectPtr& gameObject, float radius)
{
    physx::PxSphereGeometry geometry{};
    const Vector3& scale = gameObject->Transform()->Scale();
    float maxScale = Mathf::Max(Mathf::Max(scale.x, scale.y), scale.z);
    geometry.radius = Mathf::Max(radius * maxScale, Mathf::EPSILON);
    return geometry;
}

physx::PxCapsuleGeometry Glib::Internal::Geometry::CreateCapsule(const GameObjectPtr& gameObject, float height, float radius)
{
    const Vector3& scale = gameObject->Transform()->Scale();
    float maxScale = Mathf::Max(scale.x, scale.z);

    radius *= maxScale;
    physx::PxCapsuleGeometry geometry{ radius, Mathf::Max(height, Mathf::EPSILON) };
    return geometry;
}

physx::PxTriangleMesh* Glib::Internal::Geometry::CreateTriangleMesh(const Mesh& mesh, bool flipNormals)
{
    physx::PxTolerancesScale tolerancesScale;
    physx::PxCookingParams cookingParams{ tolerancesScale };

    std::vector<Vector3> vertices;
    const auto& srcVert = mesh.Vertices();
    vertices.reserve(srcVert.size());
    std::transform(srcVert.begin(), srcVert.end(), std::back_inserter(vertices), [](const auto& vert)
    {
        return Vector3{ vert.position[0], vert.position[1], vert.position[2] };
    });

    const auto& indices = mesh.Indices();
    physx::PxTriangleMeshDesc pxMeshDesc{};
    pxMeshDesc.setToDefault();
    pxMeshDesc.points.count = static_cast<physx::PxU32>(vertices.size());
    pxMeshDesc.points.stride = sizeof(std::remove_reference<decltype(vertices)>::type::value_type);
    pxMeshDesc.points.data = vertices.data();
    pxMeshDesc.triangles.count = static_cast<physx::PxU32>(indices.size() / 3);
    pxMeshDesc.triangles.stride = 3 * sizeof(std::remove_reference<decltype(indices)>::type::value_type);
    pxMeshDesc.triangles.data = indices.data();
    if (flipNormals) pxMeshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

    physx::PxDefaultMemoryOutputStream writeBuffer;
    if (!PxCookTriangleMesh(cookingParams, pxMeshDesc, writeBuffer))
    {
        Debug::Error("CookTriangleMesh failed.");
        return nullptr;
    }
    physx::PxDefaultMemoryInputData readBuffer{ writeBuffer.getData(), writeBuffer.getSize() };

    return s_physX->Physcs().createTriangleMesh(readBuffer);
}

physx::PxConvexMesh* Glib::Internal::Geometry::CreateConvexMesh(const Mesh& mesh)
{
    physx::PxTolerancesScale tolerancesScale;
    physx::PxCookingParams cookingParams{ tolerancesScale };
    cookingParams.convexMeshCookingType = physx::PxConvexMeshCookingType::Enum::eQUICKHULL;
    cookingParams.gaussMapLimit = 256;

    std::vector<Vector3> vertices;
    const auto& srcVert = mesh.Vertices();
    vertices.reserve(srcVert.size());
    std::transform(srcVert.begin(), srcVert.end(), std::back_inserter(vertices), [](const auto& vert)
    {
        return Vector3{ vert.position[0], vert.position[1], vert.position[2] };
    });

    physx::PxConvexMeshDesc pxMeshDesc{};
    pxMeshDesc.setToDefault();
    pxMeshDesc.points.count = static_cast<physx::PxU32>(vertices.size());
    pxMeshDesc.points.stride = sizeof(std::remove_reference<decltype(vertices)>::type::value_type);
    pxMeshDesc.points.data = vertices.data();
    pxMeshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxDefaultMemoryOutputStream writeBuffer;
    if (!PxCookConvexMesh(cookingParams, pxMeshDesc, writeBuffer))
    {
        Debug::Error("CookConvexMesh failed.");
        return nullptr;
    }
    physx::PxDefaultMemoryInputData readBuffer{ writeBuffer.getData(), writeBuffer.getSize() };

    return s_physX->Physcs().createConvexMesh(readBuffer);
}

physx::PxTriangleMeshGeometry Glib::Internal::Geometry::CreateTriangleMesh(const GameObjectPtr& gameObject, physx::PxTriangleMesh* const mesh)
{
    const auto& scale = gameObject->Transform()->Scale();
    physx::PxTriangleMeshGeometry geometry{};
    geometry.scale.scale.x = Mathf::Max(scale.x, Mathf::EPSILON);
    geometry.scale.scale.y = Mathf::Max(scale.y, Mathf::EPSILON);
    geometry.scale.scale.z = Mathf::Max(scale.z, Mathf::EPSILON);
    geometry.triangleMesh = mesh;
    return geometry;
}

physx::PxConvexMeshGeometry Glib::Internal::Geometry::CreateConvexMesh(const GameObjectPtr& gameObject, physx::PxConvexMesh* const mesh)
{
    const auto& scale = gameObject->Transform()->Scale();
    physx::PxConvexMeshGeometry geometry{};
    geometry.scale.scale.x = Mathf::Max(scale.x, Mathf::EPSILON);
    geometry.scale.scale.y = Mathf::Max(scale.y, Mathf::EPSILON);
    geometry.scale.scale.z = Mathf::Max(scale.z, Mathf::EPSILON);
    geometry.convexMesh = mesh;
    return geometry;
}
