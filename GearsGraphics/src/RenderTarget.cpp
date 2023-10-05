#include <RenderTarget.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Graphics::RenderTarget::Create(uint32_t width, uint32_t height, Color clearColor, DXGI_FORMAT renderFormat, DXGI_FORMAT depthFormat)
{
    width_ = width;
    height_ = height;
    clearColor_ = clearColor;

    if (renderFormat != DXGI_FORMAT_UNKNOWN)
    {
        if (!CreateRenderTargetBuffer(renderFormat)) return false;
    }
    if (depthFormat != DXGI_FORMAT_UNKNOWN)
    {
        if (!CreateDepthStencilBuffer(depthFormat)) return false;
    }

    CreateView(renderFormat, depthFormat);

    return true;
}

bool Glib::Graphics::RenderTarget::Create(uint32_t index, ComPtr<IDXGISwapChain> swapChain)
{
    if (swapChain == nullptr) return false;
    auto pool = s_dx12->DescriptorPool(Glib::Internal::Graphics::DirectX12::PoolType::RTV);
    rtvHandle_ = pool->GetHandle();
    if (rtvHandle_ == nullptr) return false;

    if (FAILED(swapChain->GetBuffer(index, IID_PPV_ARGS(renderTarget_.ReleaseAndGetAddressOf())))) return false;

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swapChain->GetDesc(&desc))) return false;

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = desc.BufferDesc.Format;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;

    s_dx12->Device()->CreateRenderTargetView(
        renderTarget_.Get(),
        &rtvDesc,
        rtvHandle_->CPU()
    );

    return true;
}

bool Glib::Graphics::RenderTarget::CreateRenderTargetBuffer(DXGI_FORMAT format)
{
    auto pool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RTV);
    rtvHandle_ = pool->GetHandle();
    if (rtvHandle_ == nullptr) return false;

    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT };
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width_, height_, 1, 1);
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    const float color[]{ 1.0f, 1.0f, 1.0f, 1.0f };
    auto clearValue = CD3DX12_CLEAR_VALUE{ format, color };

    return SUCCEEDED(s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clearValue,
        IID_PPV_ARGS(renderTarget_.ReleaseAndGetAddressOf()
    )));
}

bool Glib::Graphics::RenderTarget::CreateDepthStencilBuffer(DXGI_FORMAT format)
{
    auto pool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::DSV);
    dsvHandle_ = pool->GetHandle();
    if (dsvHandle_ == nullptr) return false;

    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT };
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width_, height_, 1, 1);
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    CD3DX12_CLEAR_VALUE clearValue{ format, 1.0f, 0 };

    return SUCCEEDED(s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(depthStencil_.ReleaseAndGetAddressOf())
    ));
}

void Glib::Graphics::RenderTarget::CreateView(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat)
{
    if (rtvHandle_ != nullptr)
    {
        D3D12_RENDER_TARGET_VIEW_DESC desc{};
        desc.Format = rtvFormat;
        desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        s_dx12->Device()->CreateRenderTargetView(
            renderTarget_.Get(),
            &desc,
            rtvHandle_->CPU()
        );
    }

    if (dsvHandle_ != nullptr && rtvFormat != DXGI_FORMAT_UNKNOWN)
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
        desc.Format = dsvFormat;
        desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        desc.Flags = D3D12_DSV_FLAG_NONE;
        s_dx12->Device()->CreateDepthStencilView(
            depthStencil_.Get(),
            &desc,
            dsvHandle_->CPU()
        );
    }
}

Glib::WeakPtr<Glib::Internal::Graphics::DescriptorHandle> Glib::Graphics::RenderTarget::RTVHandle() const
{
    return rtvHandle_;
}

Glib::WeakPtr<Glib::Internal::Graphics::DescriptorHandle> Glib::Graphics::RenderTarget::DSVHandle() const
{
    return dsvHandle_;
}

ComPtr<ID3D12Resource> Glib::Graphics::RenderTarget::RenderTargetResource() const
{
    return renderTarget_;
}

ComPtr<ID3D12Resource> Glib::Graphics::RenderTarget::DepthStencilResource() const
{
    return depthStencil_;
}

D3D12_RESOURCE_DESC Glib::Graphics::RenderTarget::RTVResourceDesc() const
{
    return renderTarget_ == nullptr ? D3D12_RESOURCE_DESC{} : renderTarget_->GetDesc();
}

D3D12_RESOURCE_DESC Glib::Graphics::RenderTarget::DSVResourceDesc() const
{
    return depthStencil_ == nullptr ? D3D12_RESOURCE_DESC{} : depthStencil_->GetDesc();
}

DXGI_FORMAT Glib::Graphics::RenderTarget::RenderTargetFormat() const
{
    return renderTarget_ == nullptr ? DXGI_FORMAT_UNKNOWN : renderTarget_->GetDesc().Format;
}

DXGI_FORMAT Glib::Graphics::RenderTarget::DepthStencilFormat() const
{
    return depthStencil_ == nullptr ? DXGI_FORMAT_UNKNOWN : depthStencil_->GetDesc().Format;
}

void Glib::Graphics::RenderTarget::AsTexture() const
{
    s_dx12->Barrier(
        renderTarget_.Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
}

void Glib::Graphics::RenderTarget::AsRenderTarget() const
{
    s_dx12->Barrier(
        renderTarget_.Get(),
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
}
