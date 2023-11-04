#pragma once

#include <string>
#include <Vector3.h>

namespace Glib
{
    struct Bone
    {
        std::string name{ "" };
        Vector3 position{ 0.0f, 0.0f, 0.0f };
        int parent{ 0 };
        int boneIndex{ -1 };
    };
}
