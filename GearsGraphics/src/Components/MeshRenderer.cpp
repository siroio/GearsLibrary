#include <Components/MeshRenderer.h>
#include <Internal/RenderingManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/DX12/DynamicConstantBuffer.h>
#include <MeshManager.h>
#include <GameObject.h>
#include <Matrix4x4.h>
#include <GLGUI.h>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_renderingManager = RenderingManager::Instance();
    auto s_graphics = GraphicsResource::Instance();
    auto s_meshManager = Glib::MeshManager::Instance();
}


Glib::MeshRenderer::MeshRenderer()
{
    constantBuffer_ = ConstantBufferAddress{};
    isEnabled_ = false;
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
    Matrix4x4 world = Matrix4x4::TRS(
        transform_->Position(),
        transform_->Rotation(),
        transform_->Scale()
    );

    auto buffer = s_dx12->GetConstantBuffer();
    constantBuffer_ = buffer->Alloc(&world, sizeof(Matrix4x4));
}

void Glib::MeshRenderer::Draw(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!isEnabled_) return;
    s_graphics->SetPipelineState(ID::MESH_PIPELINESTATE);
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(ID::MESH_WORLD_MATRIX, constantBuffer_.Address());
    s_renderingManager->SetDirectionalLightConstant(ID::MESH_DIRECTIONAL_LIGHT);
    camera->SetConstantBuffer(ID::MESH_CAMERA_CONSTANT);
    camera->SetShadowMap(ID::MESH_SHADOW_MAP);
    s_meshManager->Draw(meshID_);
}

void Glib::MeshRenderer::DrawShadow(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!isEnabled_) return;
    s_graphics->SetPipelineState(ID::MESH_SHADOW_PIPELINESTATE);
    camera->SetConstantBuffer(0);
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(1, constantBuffer_.Address());
    s_meshManager->DrawShadow(meshID_);
}

unsigned int Glib::MeshRenderer::MeshID() const
{
    return meshID_;
}

void Glib::MeshRenderer::MeshID(unsigned int id)
{
    meshID_ = id;
    isEnabled_ = s_meshManager->Contains(id);
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
