#pragma once

class Vector3;

namespace Glib
{
    struct RaycastHit;
}

namespace Glib
{
    class Physics
    {
        bool Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* hit);
    };
}
