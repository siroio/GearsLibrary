#pragma once
#include <Internal/DescriptorPool.h>
#include <ComPtr.h>
#include <dxgi1_6.h>

namespace Glib::Internal::Graphics
{
    class RenderTarget
    {
    public:
        RenderTarget() = default;
        bool Create(uint32_t index, IDXGISwapChain* swapChain);
        bool Create(uint32_t width, uint32_t height, DXGI_FORMAT format);

        DescriptorHandle* Handle() const;
        ID3D12Resource* Resource() const;
        D3D12_RESOURCE_DESC ResourceDesc() const;
        D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc() const;

    private:
        RenderTarget(const RenderTarget&) = delete;
        void operator = (const RenderTarget&) = delete;

    private:
        ComPtr<ID3D12Resource> target_{ nullptr };
        DescriptorHandle* handle_{ nullptr };
        DescriptorPool* pool_{ nullptr };
        D3D12_RENDER_TARGET_VIEW_DESC viewDesc_;
    };
}
