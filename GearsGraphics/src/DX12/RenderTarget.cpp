#include <DX12/RenderTarget.h>
#include <DX12/Internal/DirectX12.h>
#include <DX12/Internal/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Graphics::RenderTarget::Create(uint32_t index, ComPtr<IDXGISwapChain> swapChain)
{
    if (swapChain == nullptr) return false;
    pool_ = s_dx12->DescriptorPool(Glib::Internal::Graphics::DirectX12::POOLTYPE::RTV);
    handle_ = pool_->GetHandle();
    if (handle_ == nullptr) return false;

    if (FAILED(swapChain->GetBuffer(index, IID_PPV_ARGS(target_.ReleaseAndGetAddressOf())))) return false;

    DXGI_SWAP_CHAIN_DESC desc{};
    swapChain->GetDesc(&desc);

    viewDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    viewDesc_.Format = desc.BufferDesc.Format;
    viewDesc_.Texture2D.MipSlice = 0;
    viewDesc_.Texture2D.PlaneSlice = 0;

    s_dx12->Device()->CreateRenderTargetView(
        target_.Get(),
        &viewDesc_,
        handle_->CPU()
    );

    return true;
}

bool Glib::Graphics::RenderTarget::Create(uint32_t width, uint32_t height, DXGI_FORMAT format, std::shared_ptr<Glib::Internal::Graphics::DescriptorPool> pool)
{
    pool_ = pool;

    handle_ = pool_->GetHandle();
    if (handle_ == nullptr) return false;

    D3D12_HEAP_PROPERTIES heapProp{};
    heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resDesc.Alignment = 0;
    resDesc.Width = static_cast<UINT64>(width);
    resDesc.Height = static_cast<UINT64>(height);
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = format;
    resDesc.SampleDesc.Count = 1;
    resDesc.SampleDesc.Quality = 0;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    const float color[]{ 1.0f, 1.0f, 1.0f, 1.0f };
    auto clearValue = CD3DX12_CLEAR_VALUE{ format, color };

    bool failed = FAILED(s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clearValue,
        IID_PPV_ARGS(target_.ReleaseAndGetAddressOf()
    )));

    if (failed) return false;

    viewDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    viewDesc_.Format = format;
    viewDesc_.Texture2D.MipSlice = 0;
    viewDesc_.Texture2D.PlaneSlice = 0;

    s_dx12->Device()->CreateRenderTargetView(
        target_.Get(),
        &viewDesc_,
        handle_->CPU()
    );

    return true;
}

Glib::WeakPtr<Glib::Internal::Graphics::DescriptorHandle> Glib::Graphics::RenderTarget::Handle() const
{
    return handle_;
}

ComPtr<ID3D12Resource> Glib::Graphics::RenderTarget::Resource() const
{
    return target_;
}

D3D12_RESOURCE_DESC Glib::Graphics::RenderTarget::ResourceDesc() const
{
    if (target_ == nullptr) return D3D12_RESOURCE_DESC{};
    return target_->GetDesc();
}

D3D12_RENDER_TARGET_VIEW_DESC Glib::Graphics::RenderTarget::RenderTargetViewDesc() const
{
    return viewDesc_;
}
