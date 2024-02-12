#pragma once
#include <Internal/ICollider.h>
#include <Component.h>
#include <Vector3.h>

namespace physx
{
    class PxMaterial;
    class PxShape;
    class PxGeometry;
}

namespace Glib::Internal::Interface
{
    class IRigidbody;
}

namespace Glib
{
    enum class Combine
    {
        AVERAGE = 0,
        MIN = 1,
        MULTIPLY = 2,
        MAX = 3,
    };

    class Collider :
        public Component,
        public Internal::Interface::ICollider
    {
    protected:
        Collider() = default;
        virtual ~Collider();

    public:
        bool IsTrigger() const;
        void IsTrigger(bool enable);

        bool IsVisible() const;
        void IsVisible(bool enable);

        const Vector3& Center() const;
        void Center(const Vector3& center);

        float DynamicFriction() const;
        void DynamicFriction(float friction);

        float StaticFriction() const;
        void StaticFriction(float friction);

        float Bounciness() const;
        void Bounciness(float bounce);

        void SetFrictionCombine(Combine combine);
        void SetBounceCombine(Combine combine);

        WeakPtr<Internal::Interface::IRigidbody>& Rigidbody();

    protected:
        bool Initialize();
        void CreateShape(const physx::PxGeometry& geometry);
        physx::PxShape* Shape();
        void OnGUI() override;

    private:
        void SetTrigger();
        void SetVisible();
        void SyncActive() override;

    private:
        WeakPtr<Internal::Interface::IRigidbody> rigidbody_{ nullptr };
        physx::PxMaterial* material_{ nullptr };
        physx::PxShape* shape_{ nullptr };
        Combine frictionCombine_{ Combine::AVERAGE };
        Combine bounceCombine_{ Combine::AVERAGE };
        Vector3 center_{ 0.0f, 0.0f, 0.0f };
        float dynamicFriction_{ 0.6f };
        float staticFriction_{ 0.6f };
        float bounciness_{ 0.0f };
        bool isTrigger_{ false };
        bool isVisible_{ false };

    };
}
