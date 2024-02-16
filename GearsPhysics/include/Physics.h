#pragma once
#include <float.h>
#include <vector>

struct Vector3;

namespace Glib
{
    struct RaycastHit;
}

namespace Glib
{
    class Physics
    {
        bool Raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hit, float maxDistance = FLT_MAX);
        bool RaycastAll(const Vector3& origin, const Vector3& direction, std::vector<RaycastHit>& hit, float maxDistance = FLT_MAX);
    };
}
