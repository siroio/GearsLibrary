#include <Internal/PhysicsDebugRenderer.h>
#include <Internal/PhysXManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/VertexBuffer.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Vector3.h>
#include <Color.h>
#include <array>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_graphicsManager = Glib::Internal::Graphics::GraphicsResource::Instance();
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();
}

namespace
{
    struct PhysicsVertex
    {
        Vector3 position;
        Color color;
    };

    unsigned int s_vertexCount;
    std::array<PhysicsVertex, 2000> s_vertices;
    Glib::Internal::Graphics::VertexBuffer s_vertexBuffer;
}

bool Glib::Internal::Physics::PhysicsDebugRenderer::Initialize()
{
    return s_vertexBuffer.Create(sizeof(PhysicsVertex), static_cast<unsigned int>(s_vertices.size()));
}

void Glib::Internal::Physics::PhysicsDebugRenderer::Draw()
{
    if (s_vertexCount <= 0) return;
    s_vertexBuffer.Update(s_vertices.data());
    s_dx12->SetHeaps();
    s_graphicsManager->SetPipelineState(Graphics::ID::LINE_PIPELINESTATE);
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    s_vertexBuffer.SetBuffer();

    for (const auto& camera : s_cameraManager->Cameras())
    {
        if (!camera->Active()) continue;

        camera->SetRenderTarget();
        camera->SetConstantBuffer(0);

        for (unsigned int i = 0; i < s_vertexCount * 0.5f; i++)
        {
            s_dx12->CommandList()->DrawInstanced(2, 1, i * 2, 0);
        }
    }

    s_vertexCount = 0;
}


void Glib::Internal::Physics::PhysicsDebugRenderer::AddVertex(const Vector3& position, const Color& color)
{
    if (s_vertexCount >= 2000) return;
    s_vertices.at(s_vertexCount).position = position;
    s_vertices.at(s_vertexCount).color = color;
    s_vertexCount++;
}
