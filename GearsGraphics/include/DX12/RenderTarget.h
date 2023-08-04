#pragma once
#include <dxgi1_6.h>
#include <DX12/Internal/DescriptorPool.h>
#include <ComPtr.h>
#include <WeakPtr.h>

namespace Glib::Graphics
{
    class RenderTarget
    {
    public:
        RenderTarget() = default;
        bool Create(uint32_t index, ComPtr<IDXGISwapChain> swapChain);
        bool Create(uint32_t width, uint32_t height, DXGI_FORMAT format, std::shared_ptr<Internal::Graphics::DescriptorPool> pool);

        WeakPtr<Glib::Internal::Graphics::DescriptorHandle> Handle() const;
        ComPtr<ID3D12Resource> Resource() const;
        D3D12_RESOURCE_DESC ResourceDesc() const;
        D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc() const;

    private:
        RenderTarget(const RenderTarget&) = delete;
        void operator = (const RenderTarget&) = delete;

    private:
        ComPtr<ID3D12Resource> target_{ nullptr };
        std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle> handle_{ nullptr };
        WeakPtr<Internal::Graphics::DescriptorPool> pool_{ nullptr };
        D3D12_RENDER_TARGET_VIEW_DESC viewDesc_{};
    };
}
