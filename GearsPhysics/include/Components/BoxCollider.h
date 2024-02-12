#pragma once
#include <Components/Collider.h>
#include <Vector3.h>

namespace Glib
{
    class BoxCollider : public Collider
    {
    public:
        void Start();

        const Vector3& Size() const;
        void Size(const Vector3& size);

    private:
        void SyncGeometry() override;
        void OnGUI() override;

    private:
        Vector3 halfSize_{ 1.0f, 1.0f, 1.0f };
    };
}
