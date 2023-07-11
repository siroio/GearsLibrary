#include <Internal/RenderTarget.h>
#include <Internal/DirectX12.h>


namespace
{
    auto dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::RenderTarget::Create(uint32_t index, IDXGISwapChain* swapChain)
{
    if (swapChain == nullptr) return false;
    pool_ = dx12->DescriptorPool(Glib::Internal::Graphics::DirectX12::POOLTYPE::RTV);
    pool_->IncrementRef();

    handle_ = pool_->GetHandle();
    if (handle_ == nullptr) return false;

    if (FAILED(swapChain->GetBuffer(index, IID_PPV_ARGS(target_.ReleaseAndGetAddressOf())))) return false;

    DXGI_SWAP_CHAIN_DESC desc{};
    swapChain->GetDesc(&desc);

    viewDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    viewDesc_.Format = desc.BufferDesc.Format;
    viewDesc_.Texture2D.MipSlice = 0;
    viewDesc_.Texture2D.PlaneSlice = 0;

    dx12->Device()->CreateRenderTargetView(
        target_.Get(),
        &viewDesc_,
        handle_->CPU()
    );

    return true;
}

bool Glib::Internal::Graphics::RenderTarget::Create(uint32_t width, uint32_t height, DXGI_FORMAT format)
{
    pool_ = dx12->DescriptorPool(Glib::Internal::Graphics::DirectX12::POOLTYPE::RTV);
    pool_->IncrementRef();

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

    D3D12_CLEAR_VALUE clearValue{};
    clearValue.Format = format;
    clearValue.Color[0] = 1.0f;
    clearValue.Color[1] = 1.0f;
    clearValue.Color[2] = 1.0f;
    clearValue.Color[3] = 1.0f;

    bool failed = FAILED(dx12->Device()->CreateCommittedResource(
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

    dx12->Device()->CreateRenderTargetView(
        target_.Get(),
        &viewDesc_,
        handle_->CPU()
    );

    return true;
}

Glib::Internal::Graphics::DescriptorHandle* Glib::Internal::Graphics::RenderTarget::Handle() const
{
    return handle_;
}

ID3D12Resource* Glib::Internal::Graphics::RenderTarget::Resource() const
{
    return target_.Get();
}

D3D12_RESOURCE_DESC Glib::Internal::Graphics::RenderTarget::ResourceDesc() const
{
    if (target_ == nullptr) return D3D12_RESOURCE_DESC{};
    return target_->GetDesc();
}

D3D12_RENDER_TARGET_VIEW_DESC Glib::Internal::Graphics::RenderTarget::RenderTargetViewDesc() const
{
    return viewDesc_;
}
