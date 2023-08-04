#include <DX12/GraphicsResource.h>
#include <DX12/Internal/VertexBuffer.h>
#include <DX12/Internal/GraphicsPipeline.h>
#include <unordered_map>

namespace
{
    std::unordered_map<unsigned int, Glib::Internal::Graphics::VertexBuffer> s_vertexBuffers;
    std::unordered_map<unsigned int, Glib::Internal::Graphics::GraphicsPipeline> s_pipelines;
}

void Glib::Graphics::GraphicsResource::SetVertexBuffer(unsigned int id)
{
    s_vertexBuffers.at(id).SetBuffer();
}

void Glib::Graphics::GraphicsResource::SetPipelineState(unsigned int id)
{
    s_pipelines.at(id).SetPipeline();
}
