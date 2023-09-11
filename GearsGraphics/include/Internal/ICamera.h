#pragma once

struct Matrix4x4;

namespace Glib
{
    enum class CameraClearFlags;
}

namespace Glib::Internal::Interface
{
    class ICamera
    {
    protected:
        ICamera() = default;

    public:
        virtual void Draw() = 0;

        virtual CameraClearFlags ClearFlags() = 0;
        virtual Matrix4x4 ViewMatrix() const = 0;
        virtual Matrix4x4 ProjectionMatrix() const = 0;

        virtual void SetConstantBuffer(unsigned int rootParamIndex) = 0;
        virtual void SetDepthStencil() = 0;
        virtual void SetShadowMap(unsigned int rootParamIndex) = 0;
        virtual void ShadowBulr() = 0;
    };
};
