#pragma once
#include <Internal/CameraBase.h>
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <Components/Transform.h>
#include <CameraClearFlags.h>
#include <ComPtr.h>
#include <RenderTarget.h>
#include <GaussianBlur.h>
#include <Matrix4x4.h>
#include <Vector2.h>
#include <Vector3.h>
#include <Color.h>

namespace Glib
{
    class Transform;
}

namespace Glib
{
    /**
     * @brief 投影方法
    */
    enum class ProjectionType
    {
        /**
         * @brief 透視投影
        */
        Perspective,
        /**
         * @brief 平行投影
        */
        Orthographic
    };

    /**
     * @brief カメラコンポーネント
     */
    class Camera :
        public Internal::CameraBase,
        public Internal::Function::LateUpdateOrderSet<Internal::Order::LateUpdate::CAMERA>
    {
    public:
        Camera();
        void Start();
        void LateUpdate();

        const Color& BackGroundColor();
        void BackGroundColor(const Color& color);

        CameraClearFlags ClearFlags() override;
        void ClearFlags(CameraClearFlags flags);

        ProjectionType Projection() const;
        void Projection(ProjectionType type);

        float FieldOfView() const;
        void FieldOfView(float fieldOfView);

        float Near() const;
        void Near(float nearZ);

        float Far() const;
        void Far(float farZ);

        void ViewPort(const Vector2& position, const Vector2& size);

        float OrthographicSize() const;
        void OrthographicSize(float size);

        Vector3 WorldToScreenPoint(const Vector3& position);

    private:
        void InitializeRT();
        void InitializeSM();

    private:
        virtual void Draw() override;
        virtual void ViewMatrix(Matrix4x4& mat) const override;
        virtual void ProjectionMatrix(Matrix4x4& mat) const override;
        virtual void SetRenderTarget() override;
        virtual void SetConstantBuffer(unsigned int rootParamIndex) override;
        virtual void SetDepthStencil() override;
        virtual void SetShadowMap(unsigned int rootParamIndex) override;
        virtual void ExecuteShadowBulr() override;
        virtual void OnGUI() override;

    private:
        WeakPtr<Transform> transform_{ nullptr };
        Internal::Graphics::ConstantBuffer constantBuffer_{};

        // camera用RenderTarget
        Graphics::RenderTarget renderTarget_;
        std::shared_ptr<Internal::Graphics::DescriptorHandle> srvHandle_{ nullptr };

        // ShadowMap用RenderTarget
        Graphics::RenderTarget shadowMap_{};
        Graphics::GaussianBlur shadowMapBlur{};

        Color backGroundColor_{ 0.192f, 0.302f, 0.475f, 1.000f };
        CameraClearFlags clearFlags_{ CameraClearFlags::SkyBox };
        ProjectionType projectionType_{ ProjectionType::Perspective };
        float fieldOfView_{ 60.0f };
        float near_{ 0.3f };
        float far_{ 1000.0f };
        Vector2 viewPortPosition_{ 0.0f, 0.0f };
        Vector2 viewPortSize_{ 1.0f, 1.0f };
        float orthographicSize_{ 10.0f };
    };
}
