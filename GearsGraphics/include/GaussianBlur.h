#pragma once
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/DX12/GraphicsPipeline.h>
#include <ComPtr.h>
#include <memory>

enum DXGI_FORMAT;
struct ID3D12Resource;

namespace Glib::Internal::Graphics
{
    class DescriptorHandle;
}

namespace Glib::Graphics
{
    class GaussianBlur
    {
    public:
        bool Initialize(const ComPtr<ID3D12Resource>& texture, DXGI_FORMAT format);
        void Execute(float power);
        void SetTexture(unsigned int rootParamIndex);

    private:
        bool CreatePipelines(DXGI_FORMAT format);
        bool CreateBlurResource(UINT64 width, UINT height, DXGI_FORMAT format);
        void CreateViews(const ComPtr<ID3D12Resource>& texture, DXGI_FORMAT format);
    private:
        ComPtr<ID3D12Resource> xBlurResource_{ nullptr };
        ComPtr<ID3D12Resource> yBlurResource_{ nullptr };
        std::shared_ptr<Internal::Graphics::DescriptorHandle> rtvHandle_[2]{ nullptr };
        std::shared_ptr<Internal::Graphics::DescriptorHandle> srvHandle_[3]{ nullptr };
        Internal::Graphics::ConstantBuffer constantBuffer_{};
        Internal::Graphics::GraphicsPipeline pipelines_[2]{};
    };
}
