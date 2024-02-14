#pragma once
#include <Components/Collider.h>

namespace Glib
{
    class CapsuleCollider : public Collider
    {
    public:
        void Start();

        float Height() const;
        void Height(float height);

        float Radius() const;
        void Radius(float radius);

    private:
        void SyncGeometry() override;
        void OnGUI() override;

    private:
        float height_{ 1.0f };
        float radius_{ 1.0f };
    };
}
