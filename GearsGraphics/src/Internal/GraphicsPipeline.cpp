#include <Internal/GraphicsPipeline.h>
#include <Internal/DirectX12.h>
#include <d3d12.h>

namespace
{
    auto dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::GraphicsPipeline::CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc)
{
    ComPtr<ID3DBlob> signature;
    auto hr = D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signature.ReleaseAndGetAddressOf(),
        nullptr
    );
    if (FAILED(hr)) return false;

    hr = dx12->Device()->CreateRootSignature(
        0,
        signature->GetBufferPointer(),
        signature->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr)) return false;
    return true;
}

bool Glib::Internal::Graphics::GraphicsPipeline::CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
    desc.pRootSignature = rootSignature_.Get();
    auto hr = dx12->Device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pipelineState_.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) return false;
    return true;
}

void Glib::Internal::Graphics::GraphicsPipeline::SetPipeline()
{
    dx12->CommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    dx12->CommandList()->SetPipelineState(pipelineState_.Get());
}
