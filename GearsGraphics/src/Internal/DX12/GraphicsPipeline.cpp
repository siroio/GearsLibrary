#include <Internal/DX12/GraphicsPipeline.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/BlendState.h>
#include <Internal/DX12/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC Glib::Internal::Graphics::GraphicsPipeline::CreateDefaultPipelineDesc()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    desc.RasterizerState = CD3DX12_RASTERIZER_DESC{ D3D12_DEFAULT };
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

    desc.BlendState.RenderTarget[0] = BlendDesc::Create(BlendState::Default);
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

    desc.DepthStencilState.DepthEnable = true;
    desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    desc.DepthStencilState.StencilEnable = false;
    desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    return desc;
}

bool Glib::Internal::Graphics::GraphicsPipeline::CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc)
{
    ComPtr<ID3DBlob> signature{ nullptr };
    if (FAILED(D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signature.ReleaseAndGetAddressOf(),
        nullptr
        ))) return false;

    if (FAILED(s_dx12->Device()->CreateRootSignature(
        0,
        signature->GetBufferPointer(),
        signature->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf())
        ))) return false;

    return true;
}

bool Glib::Internal::Graphics::GraphicsPipeline::CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
    desc.pRootSignature = rootSignature_.Get();
    return SUCCEEDED(s_dx12->Device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pipelineState_.ReleaseAndGetAddressOf())));
}

void Glib::Internal::Graphics::GraphicsPipeline::SetPipeline()
{
    s_dx12->CommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    s_dx12->CommandList()->SetPipelineState(pipelineState_.Get());
}
