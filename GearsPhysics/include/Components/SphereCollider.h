#pragma once
#include <Components/Collider.h>
#include <Vector3.h>

namespace Glib
{
    class SphereCollider : public Collider
    {
    public:
        void Start();

        const float Radius() const;
        void Radius(float radius);

    private:
        void SyncGeometry() override;
        void OnGUI() override;

    private:
        float radius_{ 1.0f };
    };
}
