﻿#pragma once
#include <GameObjectPtr.h>
#include <Vector3.h>

namespace physx
{
    class PxBoxGeometry;
    class PxCapsuleGeometry;
}

namespace Glib::Internal
{
    class Geometory
    {
    private:
        Geometory() = default;
        ~Geometory() = default;

    public:
        static physx::PxBoxGeometry CreateBox(const GameObjectPtr& gameObject, const Vector3& size);
        static physx::PxCapsuleGeometry CreateCapsule(const GameObjectPtr& gameObject, float height, float radius);
    };
}
