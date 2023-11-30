#include <Components/SkinnedMeshRenderer.h>
#include <Internal/RenderingManager.h>
#include <Internal/DX12/GraphicsResource.h>
#include <MeshManager.h>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_renderingManager = RenderingManager::Instance();
    auto s_graphics = GraphicsResource::Instance();
    auto& s_meshManager = Glib::MeshManager::Instance();
}

Glib::SkinnedMeshRenderer::SkinnedMeshRenderer()
{

}

void Glib::SkinnedMeshRenderer::Start()
{
    WeakPtr<SkinnedMeshRenderer> renderer{ weak_from_this() };
    s_renderingManager->AddRenderer(renderer.get());
}

void Glib::SkinnedMeshRenderer::LateUpdate()
{}

void Glib::SkinnedMeshRenderer::Draw(const WeakPtr<Internal::CameraBase>& camera)
{}

void Glib::SkinnedMeshRenderer::DrawShadow(const WeakPtr<Internal::CameraBase>& camera)
{}

unsigned int Glib::SkinnedMeshRenderer::MeshID() const
{
    return 0;
}

void Glib::SkinnedMeshRenderer::MeshID(unsigned int id)
{
    meshID_ = id;
}
