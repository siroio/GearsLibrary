#include <GaussianBlur.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsPipeline.h>
#include <Internal/DX12/InputLayout.h>
#include <Internal/DX12/Shader/ShaderManager.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Vector2.h>
#include <Mathf.h>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_shaderManager = ShaderManager::Instance();
    auto s_resources = GraphicsResource::Instance();

    struct GaussianBlurConstant
    {
        float weights[8];
    };
}

bool Glib::Graphics::GaussianBlur::Initialize(const ComPtr<ID3D12Resource>& texture, DXGI_FORMAT format)
{
    auto desc = texture->GetDesc();
    if (!CreateBlurResource(desc.Width, desc.Height, desc.Format)) return false;
    CreateViews(texture, format);
    if (!CreatePipelines(format)) return false;
    if (!constantBuffer_.Create(sizeof(GaussianBlurConstant))) return false;
    return true;
}

void Glib::Graphics::GaussianBlur::Execute(float power)
{
    // 定数バッファ更新
    GaussianBlurConstant buffer{};
    float total = 0.0f;

    for (int i = 0; i < 8; i++)
    {
        buffer.weights[i] = Mathf::Exp(-0.5f * (i * i) / power);
        total += 2.0f * buffer.weights[i];
    }
    for (int i = 0; i < 8; i++)
    {
        buffer.weights[i] /= total;
    }

    constantBuffer_.Update(sizeof(buffer), &buffer);

    s_resources->SetVertexBuffer(ID::CAMERA_VERTEX);
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    float clearColor[4]{ 1.0f, 1.0f, 1.0f, 1.0f };

    {
        // 横ブラー
        s_dx12->Barrier(
            xBlurResource_.Get(),
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            D3D12_RESOURCE_STATE_RENDER_TARGET);
        pipelines_[0].SetPipeline();
        constantBuffer_.SetBuffer(0);
        s_dx12->CommandList()->OMSetRenderTargets(1, &rtvHandle_[0]->CPU(), false, nullptr);
        s_dx12->CommandList()->ClearRenderTargetView(rtvHandle_[0]->CPU(), clearColor, 0, nullptr);

        CD3DX12_VIEWPORT viewPort{ xBlurResource_.Get() };
        CD3DX12_RECT rect{ 0, 0, static_cast<LONG>(viewPort.Width), static_cast<LONG>(viewPort.Height) };
        s_dx12->CommandList()->RSSetViewports(1, &viewPort);
        s_dx12->CommandList()->RSSetScissorRects(1, &rect);

        s_dx12->CommandList()->SetGraphicsRootDescriptorTable(1, srvHandle_[0]->GPU());
        s_dx12->CommandList()->DrawInstanced(4, 1, 0, 0);

        s_dx12->Barrier(
            xBlurResource_.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }

    {
        s_dx12->Barrier(
            yBlurResource_.Get(),
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

        pipelines_[1].SetPipeline();
        constantBuffer_.SetBuffer(0);
        s_dx12->CommandList()->OMSetRenderTargets(1, &rtvHandle_[1]->CPU(), false, nullptr);
        s_dx12->CommandList()->ClearRenderTargetView(rtvHandle_[1]->CPU(), clearColor, 0, nullptr);

        // 縦ブラー
        CD3DX12_VIEWPORT viewPort{ yBlurResource_.Get() };
        CD3DX12_RECT rect{ 0, 0, static_cast<LONG>(viewPort.Width), static_cast<LONG>(viewPort.Height) };
        s_dx12->CommandList()->RSSetViewports(1, &viewPort);
        s_dx12->CommandList()->RSSetScissorRects(1, &rect);

        s_dx12->CommandList()->SetGraphicsRootDescriptorTable(1, srvHandle_[1]->GPU());
        s_dx12->CommandList()->DrawInstanced(4, 1, 0, 0);

        s_dx12->Barrier(
            yBlurResource_.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
}

void Glib::Graphics::GaussianBlur::SetTexture(unsigned int rootParamIndex)
{
    s_dx12->CommandList()->SetGraphicsRootDescriptorTable(rootParamIndex, srvHandle_[2]->GPU());
}

bool Glib::Graphics::GaussianBlur::CreatePipelines(DXGI_FORMAT format)
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[]{
        InputLayout::POSITION_2D
    };

    CD3DX12_DESCRIPTOR_RANGE range[2]{};
    range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1U, 0U);
    range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1U, 0U);

    CD3DX12_ROOT_PARAMETER rootParam[2]{};
    rootParam[0].InitAsDescriptorTable(1, &range[0]);
    rootParam[1].InitAsDescriptorTable(1, &range[1]);

    CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.Init(
        0U, D3D12_FILTER_ANISOTROPIC,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP
    );

    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.pParameters = rootParam;
    rootSigDesc.NumParameters = static_cast<UINT>(std::size(rootParam));
    rootSigDesc.pStaticSamplers = &samplerDesc;
    rootSigDesc.NumStaticSamplers = 1;

    for (auto& pipeline : pipelines_)
    {
        if (!pipeline.CreateRootSignature(rootSigDesc)) return false;
    }

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.InputLayout.NumElements = static_cast<UINT32>(std::size(inputLayout));
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.DepthStencilState.DepthEnable = false;
    pipelineDesc.RTVFormats[0] = format;

    s_shaderManager->SetVertexShader(ID::XBLUR_VERTEX_SHADER, pipelineDesc);
    s_shaderManager->SetPixelShader(ID::BLUR_PIXEL_SHADER, pipelineDesc);
    if (!pipelines_[0].CreatePipelineState(pipelineDesc)) return false;

    s_shaderManager->SetVertexShader(ID::YBLUR_VERTEX_SHADER, pipelineDesc);
    if (!pipelines_[1].CreatePipelineState(pipelineDesc)) return false;

    return true;
}

bool Glib::Graphics::GaussianBlur::CreateBlurResource(UINT64 width, UINT height, DXGI_FORMAT format)
{
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT };
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, static_cast<UINT64>(width * 0.5f), height, 1, 1);
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    float clearColor[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
    auto clearValue = CD3DX12_CLEAR_VALUE{ format, clearColor };

    // 横ブラー用
    if (FAILED(s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValue,
        IID_PPV_ARGS(xBlurResource_.ReleaseAndGetAddressOf())
        ))) return false;

    resDesc.Height = static_cast<UINT>(resDesc.Height * 0.5f);

    // 縦ブラー用
    if (FAILED(s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValue,
        IID_PPV_ARGS(yBlurResource_.ReleaseAndGetAddressOf())
        ))) return false;

    return true;
}

void Glib::Graphics::GaussianBlur::CreateViews(const ComPtr<ID3D12Resource>& texture, DXGI_FORMAT format)
{
    auto rtvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RTV);
    auto srvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RES);

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = format;

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = format;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    // Handleを取得
    for (auto& ptr : rtvHandle_)
    {
        ptr = rtvPool->GetHandle();
    }

    for (auto& ptr : srvHandle_)
    {
        ptr = srvPool->GetHandle();
    }

    // RTV作成
    s_dx12->Device()->CreateRenderTargetView(xBlurResource_.Get(), &rtvDesc, rtvHandle_[0]->CPU());
    s_dx12->Device()->CreateRenderTargetView(yBlurResource_.Get(), &rtvDesc, rtvHandle_[1]->CPU());

    // SRV作成
    s_dx12->Device()->CreateShaderResourceView(texture.Get(), &srvDesc, srvHandle_[0]->CPU());
    s_dx12->Device()->CreateShaderResourceView(xBlurResource_.Get(), &srvDesc, srvHandle_[1]->CPU());
    s_dx12->Device()->CreateShaderResourceView(yBlurResource_.Get(), &srvDesc, srvHandle_[2]->CPU());
}
