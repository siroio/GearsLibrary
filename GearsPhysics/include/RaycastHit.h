#pragma once
#include <GameObjectPtr.h>
#include <Vector3.h>

namespace Glib
{
    struct RaycastHit
    {
        GameObjectPtr gameObject{ nullptr };
        Vector3 point{ 0.0f, 0.0f, 0.0f };
        Vector3 normal{ 0.0f, 0.0f, 0.0f };
        float distance{ 0.0f };
    };
}
