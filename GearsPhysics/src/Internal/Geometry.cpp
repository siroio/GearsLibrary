#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <Mathf.h>

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
