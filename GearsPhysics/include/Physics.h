#pragma once
#include <float.h>

class Vector3;

namespace Glib
{
    struct RaycastHit;
}

namespace Glib
{
    class Physics
    {
        bool Raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hit, float maxDistance = FLT_MAX);
    };
}
