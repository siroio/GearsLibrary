#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <Mathf.h>

physx::PxBoxGeometry Glib::Internal::Geometory::CreateBox(const GameObjectPtr& gameObject, const Vector3& size)
{
    physx::PxBoxGeometry geometry{};
    const Vector3& scale = gameObject->Transform()->Scale();
    geometry.halfExtents.x = size.x * Mathf::Max(scale.x, Mathf::EPSILON);
    geometry.halfExtents.y = size.y * Mathf::Max(scale.y, Mathf::EPSILON);
    geometry.halfExtents.z = size.z * Mathf::Max(scale.z, Mathf::EPSILON);
    return geometry;
}

physx::PxCapsuleGeometry Glib::Internal::Geometory::CreateCapsule(const GameObjectPtr& gameObject, float halfHeight, float radius)
{
    physx::PxCapsuleGeometry geometry{};
    geometry.halfHeight = halfHeight;
    geometry.radius = radius;
    return geometry;
}
