#include <Internal/PhysicsDebugRenderer.h>
#include <Internal/JoltPhysicsManager.h>
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
    renderer_ = std::make_unique<InternalRenderer>();
    return s_vertexBuffer.Create(sizeof(PhysicsVertex), static_cast<unsigned int>(s_vertices.size()));
}

void Glib::Internal::Physics::PhysicsDebugRenderer::Draw()
{
    if (s_vertexCount <= 0) return;
    s_vertexBuffer.Update(s_vertices.data());
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

JPH::DebugRenderer& Glib::Internal::Physics::PhysicsDebugRenderer::Renderer() const
{
    return *renderer_;
}

void Glib::Internal::Physics::PhysicsDebugRenderer::AddVertex(const Vector3& position, const Color& color)
{
    if (s_vertexCount >= 2000) return;
    s_vertices.at(s_vertexCount).position = position;
    s_vertices.at(s_vertexCount).color = color;
    s_vertexCount++;
}

void Glib::Internal::Physics::PhysicsDebugRenderer::InternalRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
{
    const auto& from = JoltPhysicsManager::ToVector3(inFrom);
    const auto& to = JoltPhysicsManager::ToVector3(inTo);
    const auto& color = JoltPhysicsManager::ToColor(inColor);
    AddVertex(from, color);
    AddVertex(to, color);
}

void Glib::Internal::Physics::PhysicsDebugRenderer::InternalRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
    const auto& v1 = JoltPhysicsManager::ToVector3(inV1);
    const auto& v2 = JoltPhysicsManager::ToVector3(inV2);
    const auto& v3 = JoltPhysicsManager::ToVector3(inV3);
    const auto& color = JoltPhysicsManager::ToColor(inColor);
    AddVertex(v1, color);
    AddVertex(v2, color);
    AddVertex(v3, color);
}

JPH::DebugRenderer::Batch Glib::Internal::Physics::PhysicsDebugRenderer::InternalRenderer::CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount)
{
    return Batch();
}

JPH::DebugRenderer::Batch Glib::Internal::Physics::PhysicsDebugRenderer::InternalRenderer::CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount)
{
    return Batch();
}

void Glib::Internal::Physics::PhysicsDebugRenderer::InternalRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode)
{}

void Glib::Internal::Physics::PhysicsDebugRenderer::InternalRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
{}
