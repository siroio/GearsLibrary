#pragma once
#include <dxgi1_6.h>
#include <ComPtr.h>
#include <WeakPtr.h>
#include <Internal/DescriptorPool.h>


namespace Glib::Internal::Graphics
{
    class RenderTarget
    {
    public:
        RenderTarget() = default;
        bool Create(uint32_t index, ComPtr<IDXGISwapChain> swapChain);
        bool Create(uint32_t width, uint32_t height, DXGI_FORMAT format, std::shared_ptr<DescriptorPool> pool);

        WeakPtr<DescriptorHandle> Handle() const;
        ComPtr<ID3D12Resource> Resource() const;
        D3D12_RESOURCE_DESC ResourceDesc() const;
        D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc() const;

    private:
        RenderTarget(const RenderTarget&) = delete;
        void operator = (const RenderTarget&) = delete;

    private:
        ComPtr<ID3D12Resource> target_{ nullptr };
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
        std::shared_ptr<DescriptorPool> pool_{ nullptr };
        D3D12_RENDER_TARGET_VIEW_DESC viewDesc_{};
    };
}
