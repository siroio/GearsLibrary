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

bool Glib::Physics::RaycastAll(const Vector3& origin, const Vector3& direction, std::vector<RaycastHit>& hit, float maxDistance)
{
    return s_physX->RaycastAll(origin, direction, hit, maxDistance);
}

void Glib::Physics::SetCollisionFlag(unsigned int layer1, unsigned int layer2, bool enable)
{
    s_physX->SetCollisionFlags(layer1, layer2, enable);
}
