#pragma once
#include <Internal/CameraBase.h>
#include <Internal/DX12/ConstantBuffer.h>
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
     * @brief ���e���@
    */
    enum class ProjectionType
    {
        /**
         * @brief �������e
        */
        Perspective,
        /**
         * @brief ���s���e
        */
        Orthographic
    };

    /**
     * @brief �J�����R���|�[�l���g
     */
    class Camera : public Internal::CameraBase
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
        virtual Matrix4x4 ViewMatrix() const override;
        virtual Matrix4x4 ProjectionMatrix() const override;
        virtual void SetRenderTarget() override;
        virtual void SetConstantBuffer(unsigned int rootParamIndex) override;
        virtual void SetDepthStencil() override;
        virtual void SetShadowMap(unsigned int rootParamIndex) override;
        virtual void ExecuteShadowBulr() override;

    private:
        WeakPtr<Transform> transform_{ nullptr };
        Internal::Graphics::ConstantBuffer constantBuffer_{};

        ComPtr<ID3D12Resource> renderTarget_{ nullptr };
        std::shared_ptr<Internal::Graphics::DescriptorHandle> rtvHandle_{ nullptr };
        std::shared_ptr<Internal::Graphics::DescriptorHandle> srvHandle_{ nullptr };

        ComPtr<ID3D12Resource> depthStencil_{ nullptr };
        std::shared_ptr<Internal::Graphics::DescriptorHandle> dsvHandle_{ nullptr };

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
