#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/VertexBuffer.h>
#include <Internal/DX12/GraphicsPipeline.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/DX12/InputLayout.h>
#include <Internal/DX12/Shader/ShaderManager.h>
#include <Vector2.h>
#include <unordered_map>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_shader = Glib::Internal::Graphics::ShaderManager::Instance();
}

namespace
{
    std::unordered_map<unsigned int, Glib::Internal::Graphics::VertexBuffer> s_vertexBuffers;
    std::unordered_map<unsigned int, Glib::Internal::Graphics::GraphicsPipeline> s_pipelines;
    std::unordered_map<unsigned int, ComPtr<ID3D12Resource>> s_textureResources;
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle>> s_srvHandles;
}

bool Glib::Internal::Graphics::GraphicsResource::Initialize()
{
    return true;
}

void Glib::Internal::Graphics::GraphicsResource::SetVertexBuffer(unsigned int id)
{
    s_vertexBuffers.at(id).SetBuffer();
}

void Glib::Internal::Graphics::GraphicsResource::SetPipelineState(unsigned int id)
{
    s_pipelines.at(id).SetPipeline();
}

void Glib::Internal::Graphics::GraphicsResource::SetTexture(unsigned int id, unsigned int rootParameterIndex)
{
    if (!s_textureResources.contains(id)) return;
    s_dx12->CommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, s_srvHandles.at(id)->GPU());
}

bool Glib::Internal::Graphics::GraphicsResource::CreateTexture(unsigned int id, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    const UINT64 width = 4;
    const UINT64 height = 4;
    auto texHeapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0 };
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1);

    // リソースの作成
    auto result = s_dx12->Device()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(s_textureResources[id].ReleaseAndGetAddressOf())
    );

    if (FAILED(result)) return false;

    // 幅 * 高さ * RGBA 分確保
    unsigned char texData[width * height * 4]{};
    for (int i = 0; i < std::size(texData); i++)
    {
        auto idx = i % 4;
        if (idx == 0) texData[i] = r;
        else if (idx == 1) texData[i] = g;
        else if (idx == 2) texData[i] = b;
        else if (idx == 3) texData[i] = a;
        else texData[i] = 0xFF;
    }

    // データ転送
    result = s_textureResources.at(id)->WriteToSubresource(
        0,
        nullptr,
        texData,
        width * 4,
        (UINT)std::size(texData)
    );

    if (FAILED(result)) return false;

    auto handle = s_dx12->DescriptorPool(DirectX12::PoolType::RES);
    s_srvHandles[id] = handle->GetHandle();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = resDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    s_dx12->Device()->CreateShaderResourceView(
        s_textureResources.at(id).Get(),
        &srvDesc,
        s_srvHandles.at(id)->CPU()
    );

    return true;
}

bool Glib::Internal::Graphics::GraphicsResource::CreateCameraVertexBuffer()
{
    // 頂点バッファ作成
    if (!s_vertexBuffers[ID::CAMERA_VERTEX].Create(sizeof(Vector2), 4)) return false;

    // 頂点
    Vector2 vertices[]
    {
        Vector2 { 0.0f, 1.0f },
        Vector2 { 0.0f, 0.0f },
        Vector2 { 1.0f, 1.0f },
        Vector2 { 1.0f, 0.0f }
    };

    // 更新
    s_vertexBuffers.at(ID::CAMERA_VERTEX).Update(vertices);

    return true;
}

bool Glib::Internal::Graphics::GraphicsResource::CreateImageVertexBuffer()
{
    // 頂点バッファ作成
    if (!s_vertexBuffers[ID::IMAGE_VERTEX].Create(sizeof(Vector2), 4)) return false;

    // 頂点
    Vector2 vertices[]
    {
        Vector2 { 0.0f, 1.0f },
        Vector2 { 0.0f, 0.0f },
        Vector2 { 1.0f, 1.0f },
        Vector2 { 1.0f, 0.0f }
    };

    // 更新
    s_vertexBuffers.at(ID::IMAGE_VERTEX).Update(vertices);

    return true;
}

bool Glib::Internal::Graphics::GraphicsResource::CreateWhiteTexture()
{
    return CreateTexture(ID::WHITE_TEXTURE, 255, 255, 255, 255);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateNormalMapTexture()
{
    return CreateTexture(ID::NORMAL_MAP_TEXTURE, 128, 128, 255, 255);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateCameraPipelineState()
{
    auto layout = InputLayout::POSITION_2D;

    CD3DX12_DESCRIPTOR_RANGE range{};
    range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParam{};
    rootParam.InitAsDescriptorTable(1, &range);

    CD3DX12_STATIC_SAMPLER_DESC sampler{};
    sampler.Init(0);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = 1;
    rootSigDesc.NumStaticSamplers = 1;
    rootSigDesc.pParameters = &rootParam;
    rootSigDesc.pStaticSamplers = &sampler;

    if (!s_pipelines[ID::CAMERA_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;
    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    pipelineDesc.InputLayout.pInputElementDescs = &layout;
    pipelineDesc.InputLayout.NumElements = 1;
    pipelineDesc.DepthStencilState.DepthEnable = true;
    // シェーダーのセット

    // パイプラインステート作成
    if (!s_pipelines.at(ID::CAMERA_PIPELINESTATE).CreatePipelineState(pipelineDesc)) return false;

    return true;
}

bool Glib::Internal::Graphics::GraphicsResource::CreateSpritePipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[2]
    {
        InputLayout::POSITION_2D,
        InputLayout::TEXCOORD
    };

    CD3DX12_DESCRIPTOR_RANGE range[3]{};
    range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    CD3DX12_ROOT_PARAMETER rootParams[3]{};
    rootParams[0].InitAsDescriptorTable(1, &range[0]);
    rootParams[1].InitAsDescriptorTable(1, &range[1]);
    rootParams[2].InitAsDescriptorTable(1, &range[2]);

    CD3DX12_STATIC_SAMPLER_DESC sampler{};
    sampler.Init(0);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT>(std::size(rootParams));
    rootSigDesc.NumStaticSamplers = 1;
    rootSigDesc.pParameters = rootParams;
    rootSigDesc.pStaticSamplers = &sampler;

    if (!s_pipelines[ID::SPRITE_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;
    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //　カリングなし
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = static_cast<UINT>(std::size(inputLayout));
    pipelineDesc.DepthStencilState.DepthEnable = true;
    // シェーダーのセット
    s_shader->SetPixelShader(ID::SPRITE_SHADER, pipelineDesc);
    s_shader->SetVertexShader(ID::SPRITE_SHADER, pipelineDesc);

    // パイプラインステート作成
    return s_pipelines.at(ID::SPRITE_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}
