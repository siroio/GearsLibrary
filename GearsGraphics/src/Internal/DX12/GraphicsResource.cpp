#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/Shader/ShaderManager.h>
#include <Internal/DX12/VertexBuffer.h>
#include <Internal/DX12/GraphicsPipeline.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/DX12/InputLayout.h>
#include <Internal/DX12/BlendState.h>
#include <Vector2.h>
#include <unordered_map>

// TODO: PipelineSettings

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
    if (!CreateCameraVertexBuffer()) return false;
    if (!CreateImageVertexBuffer()) return false;

    if (!CreateCameraPipelineState()) return false;
    if (!CreateSpritePipelineState()) return false;
    if (!CreateImagePipelineState()) return false;
    if (!CreateLinePipelineState()) return false;
    if (!CreateMeshPipelineState()) return false;
    if (!CreateMeshShadowPipelineState()) return false;
    if (!CreateSkinnedMeshPipelineState()) return false;
    if (!CreateSkinnedMeshShadowPipelineState()) return false;

    if (!CreateWhiteTexture()) return false;
    if (!CreateMagentaTexture()) return false;
    if (!CreateNormalMapTexture()) return false;

    return true;
}

void Glib::Internal::Graphics::GraphicsResource::Finalize()
{
    s_vertexBuffers.clear();
    s_pipelines.clear();
    s_textureResources.clear();
    s_srvHandles.clear();
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
    static constexpr UINT64 width = 4;
    static constexpr UINT64 height = 4;
    static constexpr UINT64 datasize = width * height * 4;
    CD3DX12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_CUSTOM, 0U, 0U };
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1U, 1U);
    // リソースの作成
    auto result = s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(s_textureResources[id].ReleaseAndGetAddressOf())
    );
    if (FAILED(result)) return false;

    std::array<unsigned char, datasize> texData{};
    for (int i = 0; i < texData.size(); i++)
    {
        int idx = i % 4;
        switch (idx)
        {
            case 0: texData.at(i) = r; break;
            case 1: texData.at(i) = g; break;
            case 2: texData.at(i) = b; break;
            case 3: texData.at(i) = a; break;
        }
    }

    // データ転送
    result = s_textureResources.at(id)->WriteToSubresource(
        0,
        nullptr,
        texData.data(),
        static_cast<UINT>(width * 4),
        static_cast<UINT>(datasize)
    );

    if (FAILED(result)) return false;

    auto handle = s_dx12->DescriptorPool(DirectX12::PoolType::RES);
    s_srvHandles[id] = handle->GetHandle();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = resDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
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

bool Glib::Internal::Graphics::GraphicsResource::CreateMagentaTexture()
{
    return CreateTexture(ID::MAGENTA_TEXTURE, 255, 0, 255, 255);
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
    pipelineDesc.DepthStencilState.DepthEnable = false;

    // シェーダーのセット
    s_shader->SetVertexShader(ID::CAMERA_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::CAMERA_SHADER, pipelineDesc);

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

    CD3DX12_DESCRIPTOR_RANGE range{};
    range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParams[3]{};
    rootParams[0].InitAsDescriptorTable(1, &range);
    rootParams[1].InitAsConstantBufferView(0);
    rootParams[2].InitAsConstantBufferView(1);

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
    pipelineDesc.BlendState.RenderTarget[0] = BlendDesc::Create(BlendState::Alpha);
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = static_cast<UINT>(std::size(inputLayout));
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

    // シェーダーのセット
    s_shader->SetPixelShader(ID::SPRITE_SHADER, pipelineDesc);
    s_shader->SetVertexShader(ID::SPRITE_SHADER, pipelineDesc);

    // パイプラインステート作成
    return s_pipelines.at(ID::SPRITE_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateImagePipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout = InputLayout::POSITION_2D;

    CD3DX12_DESCRIPTOR_RANGE range{};
    range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParams[2]{};
    rootParams[0].InitAsDescriptorTable(1, &range);
    rootParams[1].InitAsConstantBufferView(0);

    CD3DX12_STATIC_SAMPLER_DESC sampler{};
    sampler.Init(0);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT>(std::size(rootParams));
    rootSigDesc.NumStaticSamplers = 1;
    rootSigDesc.pParameters = rootParams;
    rootSigDesc.pStaticSamplers = &sampler;

    if (!s_pipelines[ID::IMAGE_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    pipelineDesc.BlendState.RenderTarget[0] = BlendDesc::Create(BlendState::Alpha);
    pipelineDesc.InputLayout.pInputElementDescs = &inputLayout;
    pipelineDesc.InputLayout.NumElements = 1;
    pipelineDesc.DepthStencilState.DepthEnable = false;

    // シェーダーのセット
    s_shader->SetVertexShader(ID::IMAGE_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::IMAGE_SHADER, pipelineDesc);

    return s_pipelines.at(ID::IMAGE_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateLinePipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        InputLayout::POSITION_3D,
        InputLayout::COLOR
    };

    CD3DX12_ROOT_PARAMETER rootParam{};
    rootParam.InitAsConstantBufferView(0);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = 1;
    rootSigDesc.pParameters = &rootParam;

    if (!s_pipelines[ID::LINE_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    pipelineDesc.BlendState.RenderTarget[0] = BlendDesc::Create(BlendState::Alpha);
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = static_cast<UINT32>(std::size(inputLayout));
    pipelineDesc.DepthStencilState.DepthEnable = true;
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    pipelineDesc.DepthStencilState.StencilEnable = false;
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

    // シェーダーのセット
    s_shader->SetVertexShader(ID::LINE_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::LINE_SHADER, pipelineDesc);

    return s_pipelines.at(ID::LINE_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateMeshPipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        InputLayout::POSITION_3D,
        InputLayout::NORMAL,
        InputLayout::TEXCOORD,
        InputLayout::BONEINDEX,
        InputLayout::BONEWEIGHT,
        InputLayout::TANGENT,
    };

    // マテリアルのみ個別の定数バッファ
    CD3DX12_DESCRIPTOR_RANGE range[4]{};
    range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
    range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
    range[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);

    CD3DX12_ROOT_PARAMETER rootParams[7]{};
    rootParams[ID::MESH_ALBEDO].InitAsDescriptorTable(1, &range[0]);
    rootParams[ID::MESH_CAMERA_CONSTANT].InitAsConstantBufferView(0);
    rootParams[ID::MESH_WORLD_MATRIX].InitAsConstantBufferView(1);
    rootParams[ID::MESH_MATERIAL_BUFFER].InitAsDescriptorTable(1, &range[3]);
    rootParams[ID::MESH_DIRECTIONAL_LIGHT].InitAsConstantBufferView(4);
    rootParams[ID::MESH_SHADOW_MAP].InitAsDescriptorTable(1, &range[1]);
    rootParams[ID::MESH_NORMAL_MAP].InitAsDescriptorTable(1, &range[2]);

    CD3DX12_STATIC_SAMPLER_DESC sampler[2]{};
    sampler[0].Init(0);
    sampler[1].Init(1, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                    D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                    D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                    D3D12_TEXTURE_ADDRESS_MODE_BORDER);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT32>(std::size(rootParams));
    rootSigDesc.NumStaticSamplers = static_cast<UINT32>(std::size(sampler));
    rootSigDesc.pParameters = rootParams;
    rootSigDesc.pStaticSamplers = sampler;

    if (!s_pipelines[ID::MESH_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = static_cast<UINT32>(std::size(inputLayout));

    // シェーダーのセット
    s_shader->SetVertexShader(ID::MESH_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::MESH_SHADER, pipelineDesc);

    return s_pipelines.at(ID::MESH_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateMeshShadowPipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        InputLayout::POSITION_3D,
        InputLayout::NORMAL,
        InputLayout::TEXCOORD,
        InputLayout::BONEINDEX,
        InputLayout::BONEWEIGHT
    };

    CD3DX12_ROOT_PARAMETER rootParams[2]{};
    rootParams[0].InitAsConstantBufferView(0);
    rootParams[1].InitAsConstantBufferView(1);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT32>(std::size(rootParams));
    rootSigDesc.pParameters = rootParams;

    if (!s_pipelines[ID::MESH_SHADOW_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.InputLayout.NumElements = static_cast<UINT32>(std::size(inputLayout));
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32_FLOAT;
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;

    // シェーダーのセット
    s_shader->SetVertexShader(ID::MESH_SHADOW_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::MESH_SHADOW_SHADER, pipelineDesc);

    return s_pipelines.at(ID::MESH_SHADOW_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateSkinnedMeshPipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        InputLayout::POSITION_3D,
        InputLayout::NORMAL,
        InputLayout::TEXCOORD,
        InputLayout::BONEINDEX,
        InputLayout::BONEWEIGHT,
        InputLayout::TANGENT
    };

    // マテリアルのみ個別の定数バッファ
    CD3DX12_DESCRIPTOR_RANGE range[4]{};
    range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
    range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
    range[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);

    CD3DX12_ROOT_PARAMETER rootParams[8]{};
    rootParams[ID::MESH_ALBEDO].InitAsDescriptorTable(1, &range[0]);
    rootParams[ID::SKINNED_MESH_CAMERA_CONSTANT].InitAsConstantBufferView(0);
    rootParams[ID::SKINNED_MESH_WORLD_MATRIX].InitAsConstantBufferView(1);
    rootParams[ID::MESH_MATERIAL_BUFFER].InitAsDescriptorTable(1, &range[3]);
    rootParams[ID::SKINNED_MESH_SKINNED_MATRIX].InitAsConstantBufferView(3);
    rootParams[ID::SKINNED_MESH_DIRECTIONAL_LIGHT].InitAsConstantBufferView(4);
    rootParams[ID::SKINNED_MESH_SHADOW_MAP].InitAsDescriptorTable(1, &range[1]);
    rootParams[ID::MESH_NORMAL_MAP].InitAsDescriptorTable(1, &range[2]);

    CD3DX12_STATIC_SAMPLER_DESC sampler[2]{};
    sampler[0].Init(0);
    sampler[1].Init(1, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                    D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                    D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                    D3D12_TEXTURE_ADDRESS_MODE_BORDER);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT32>(std::size(rootParams));
    rootSigDesc.NumStaticSamplers = static_cast<UINT32>(std::size(sampler));
    rootSigDesc.pParameters = rootParams;
    rootSigDesc.pStaticSamplers = sampler;

    if (!s_pipelines[ID::SKINNED_MESH_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.InputLayout.NumElements = static_cast<UINT32>(std::size(inputLayout));
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;

    // シェーダーのセット
    s_shader->SetVertexShader(ID::SKINNED_MESH_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::SKINNED_MESH_SHADER, pipelineDesc);

    return s_pipelines.at(ID::SKINNED_MESH_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}

bool Glib::Internal::Graphics::GraphicsResource::CreateSkinnedMeshShadowPipelineState()
{
    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        InputLayout::POSITION_3D,
        InputLayout::NORMAL,
        InputLayout::TEXCOORD,
        InputLayout::BONEINDEX,
        InputLayout::BONEWEIGHT
    };

    CD3DX12_ROOT_PARAMETER rootParams[3]{};
    rootParams[0].InitAsConstantBufferView(0);
    rootParams[1].InitAsConstantBufferView(1);
    rootParams[2].InitAsConstantBufferView(3);

    // ルートシグネチャ作成
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT32>(std::size(rootParams));
    rootSigDesc.pParameters = rootParams;

    if (!s_pipelines[ID::SKINNED_MESH_SHADOW_PIPELINESTATE].CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.InputLayout.NumElements = static_cast<UINT32>(std::size(inputLayout));
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32_FLOAT;
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;

    // シェーダーのセット
    s_shader->SetVertexShader(ID::SKINNED_MESH_SHADOW_SHADER, pipelineDesc);
    s_shader->SetPixelShader(ID::SKINNED_MESH_SHADOW_SHADER, pipelineDesc);

    return s_pipelines.at(ID::SKINNED_MESH_SHADOW_PIPELINESTATE).CreatePipelineState(pipelineDesc);
}
