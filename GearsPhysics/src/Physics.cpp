#include <Physics.h>
#include <Internal/PhysXManager.h>

namespace
{
    const auto& s_physX = Glib::Internal::Physics::PhysXManager::Instance();
}

bool Glib::Physics::Raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hit, float maxDistance)
{
    return s_physX->Raycast(origin, direction, hit, maxDistance);
}
