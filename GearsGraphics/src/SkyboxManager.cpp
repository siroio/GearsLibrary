#include <SkyboxManager.h>
#include <unordered_map>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/Shader/ShaderManager.h>
#include <Internal/DX12/VertexBuffer.h>
#include <Internal/DX12/InputLayout.h>
#include <Internal/DX12/GraphicsPipeline.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <CameraClearFlags.h>
#include <Skybox.h>
#include <Vector3.h>
#include <Vector2.h>

namespace
{
    std::unordered_map<unsigned int, Glib::Skybox> s_skyboxs;

    unsigned int s_drawSkyboxId{ UINT_MAX };

    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_shader = Glib::Internal::Graphics::ShaderManager::Instance();
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();

    Glib::Internal::Graphics::GraphicsPipeline s_pipeline;

    Glib::Internal::Graphics::VertexBuffer s_vertexBuffer;

    struct SkyboxVertex
    {
        Vector3 position;
        Vector2 uv;
    };
}

bool Glib::SkyboxManager::Load(unsigned int id, std::string_view up, std::string_view down, std::string_view left, std::string_view right, std::string_view front, std::string_view back)
{
    Skybox skybox{};
    if (!skybox.Load(up, down, left, right, front, back)) return false;
    s_skyboxs.emplace(id, std::move(skybox));
    return true;
}

void Glib::SkyboxManager::SetSkybox(unsigned int id)
{
    s_drawSkyboxId = id;
}

bool Glib::SkyboxManager::Initialize()
{
    // スカイボックス用頂点
    SkyboxVertex vertices[]
    {
        // Y+
        { Vector3{ -0.5f, 0.5f,  0.5f }, Vector2{ 0.0f, 1.0f } },
        { Vector3{ -0.5f, 0.5f, -0.5f }, Vector2{ 0.0f, 0.0f } },
        { Vector3{  0.5f, 0.5f,  0.5f }, Vector2{ 1.0f, 1.0f } },
        { Vector3{  0.5f, 0.5f, -0.5f }, Vector2{ 1.0f, 0.0f } },

        // Y-
        { Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{ 0.0f, 1.0f } },
        { Vector3{ -0.5f, -0.5f,  0.5f }, Vector2{ 0.0f, 0.0f } },
        { Vector3{  0.5f, -0.5f, -0.5f }, Vector2{ 1.0f, 1.0f } },
        { Vector3{  0.5f, -0.5f,  0.5f }, Vector2{ 1.0f, 0.0f } },

        // X+
        { Vector3{ 0.5f, -0.5f,  0.5f }, Vector2{ 0.0f, 1.0f } },
        { Vector3{ 0.5f,  0.5f,  0.5f }, Vector2{ 0.0f, 0.0f } },
        { Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{ 1.0f, 1.0f } },
        { Vector3{ 0.5f,  0.5f, -0.5f }, Vector2{ 1.0f, 0.0f } },

        // X-
        { Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{ 0.0f, 1.0f } },
        { Vector3{ -0.5f,  0.5f, -0.5f }, Vector2{ 0.0f, 0.0f } },
        { Vector3{ -0.5f, -0.5f,  0.5f }, Vector2{ 1.0f, 1.0f } },
        { Vector3{ -0.5f,  0.5f,  0.5f }, Vector2{ 1.0f, 0.0f } },

        // Z+
        { Vector3{ -0.5f, -0.5f, 0.5f }, Vector2{ 0.0f, 1.0f } },
        { Vector3{ -0.5f,  0.5f, 0.5f }, Vector2{ 0.0f, 0.0f } },
        { Vector3{  0.5f, -0.5f, 0.5f }, Vector2{ 1.0f, 1.0f } },
        { Vector3{  0.5f,  0.5f, 0.5f }, Vector2{ 1.0f, 0.0f } },

        // Z-
        { Vector3{  0.5f, -0.5f, -0.5f }, Vector2{ 0.0f, 1.0f } },
        { Vector3{  0.5f,  0.5f, -0.5f }, Vector2{ 0.0f, 0.0f } },
        { Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{ 1.0f, 1.0f } },
        { Vector3{ -0.5f,  0.5f, -0.5f }, Vector2{ 1.0f, 0.0f } },
    };

    //頂点バッファ作成
    if (!s_vertexBuffer.Create(sizeof(SkyboxVertex), static_cast<unsigned int>(std::size(vertices)))) return false;
    s_vertexBuffer.Update(vertices);

    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        Internal::Graphics::InputLayout::POSITION_3D,
        Internal::Graphics::InputLayout::TEXCOORD,
    };

    // パイプライン作成
    // テクスチャ 定数バッファ
    CD3DX12_DESCRIPTOR_RANGE range[2]{};
    range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParams[2]{};
    rootParams[0].InitAsDescriptorTable(1U, &range[0]);
    rootParams[1].InitAsDescriptorTable(1U, &range[1]);

    CD3DX12_STATIC_SAMPLER_DESC sampler{};
    sampler.Init(0,
                 D3D12_FILTER_ANISOTROPIC,
                 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                 D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                 D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

    D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSigDesc.NumParameters = static_cast<UINT>(std::size(rootParams));
    rootSigDesc.NumStaticSamplers = 1;
    rootSigDesc.pParameters = rootParams;
    rootSigDesc.pStaticSamplers = &sampler;

    if (!s_pipeline.CreateRootSignature(rootSigDesc)) return false;

    auto pipelineDesc = Internal::Graphics::GraphicsPipeline::CreateDefaultPipelineDesc();
    pipelineDesc.DepthStencilState.DepthEnable = false;
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = static_cast<UINT>(std::size(inputLayout));
    s_shader->SetVertexShader(Internal::Graphics::ID::SKYBOX_SHADER, pipelineDesc);
    s_shader->SetPixelShader(Internal::Graphics::ID::SKYBOX_SHADER, pipelineDesc);
    return s_pipeline.CreatePipelineState(pipelineDesc);
}

void Glib::SkyboxManager::Draw()
{
    if (!s_skyboxs.contains(s_drawSkyboxId)) return;
    s_vertexBuffer.SetBuffer();
    s_pipeline.SetPipeline();
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // スカイボックスを描画
    for (const auto& camera : s_cameraManager->Cameras())
    {
        if (!camera->Active() || (camera->ClearFlags() == CameraClearFlags::Color)) continue;

        camera->SetRenderTarget();
        camera->SetConstantBuffer(1);
        s_skyboxs.at(s_drawSkyboxId).Draw();
    }
}
