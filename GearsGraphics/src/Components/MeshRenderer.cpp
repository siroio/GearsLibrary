#include <Components/MeshRenderer.h>
#include <Internal/RenderingManager.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <MeshManager.h>
#include <GameObject.h>
#include <Matrix4x4.h>
#include <GLGUI.h>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_renderingManager = RenderingManager::Instance();
    auto s_graphics = GraphicsResource::Instance();
    auto& s_meshManager = Glib::MeshManager::Instance();

    struct MeshConstant
    {
        Matrix4x4 world;
    };
}


Glib::MeshRenderer::MeshRenderer()
{
    isEnabled_ = false;
    constantBuffer_.Create(sizeof(MeshConstant));
}

void Glib::MeshRenderer::Start()
{
    transform_ = GameObject()->Transform();
    const auto renderer = WeakPtr<MeshRenderer>{ weak_from_this() };
    s_renderingManager->AddRenderer(renderer.get());
}

void Glib::MeshRenderer::LateUpdate()
{
    if (!isEnabled_) return;
    MeshConstant buffer;
    buffer.world = Matrix4x4::TRS(
        transform_->Position(),
        transform_->Rotation(),
        transform_->Scale()
    );

    constantBuffer_.Update(sizeof(buffer), &buffer);
}

void Glib::MeshRenderer::Draw(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!isEnabled_) return;
    s_graphics->SetPipelineState(ID::MESH_PIPELINESTATE);
    constantBuffer_.SetBuffer(ID::MESH_WORLD_MATRIX);
    s_renderingManager->SetDirectionalLightConstant(ID::MESH_DIRECTIONAL_LIGHT);
    camera->SetConstantBuffer(ID::MESH_CAMERA_CONSTANT);
    camera->SetShadowMap(ID::MESH_SHADOW_MAP);
    s_meshManager.Draw(meshID_);
}

void Glib::MeshRenderer::DrawShadow(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!isEnabled_) return;
    MeshConstant buffer;
    buffer.world = Matrix4x4::TRS(
        transform_->Position(),
        transform_->Rotation(),
        transform_->Scale()
    );

    constantBuffer_.Update(sizeof(buffer), &buffer);
    s_graphics->SetPipelineState(ID::MESH_SHADOW_PIPELINESTATE);
    camera->SetConstantBuffer(0);
    constantBuffer_.SetBuffer(1);
    s_meshManager.DrawShadow(meshID_);

}

unsigned int Glib::MeshRenderer::MeshID() const
{
    return meshID_;
}

void Glib::MeshRenderer::MeshID(unsigned int id)
{
    meshID_ = id;
    isEnabled_ = s_meshManager.Contains(id);
}

void Glib::MeshRenderer::OnGUI()
{
    Component::OnGUI();
    int meshID = MeshID();
    if (GLGUI::InputInt("MeshID", &meshID))
    {
        MeshID(meshID);
    }
}
