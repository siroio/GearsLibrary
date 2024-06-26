﻿#include <Components/SkinnedMeshRenderer.h>
#include <Internal/RenderingManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/DX12/DynamicConstantBuffer.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <MeshManager.h>
#include <Mathf.h>
#include <GLGUI.h>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_renderingManager = RenderingManager::Instance();
    auto s_graphics = GraphicsResource::Instance();
    auto s_meshManager = Glib::MeshManager::Instance();
}

void Glib::SkinnedMeshRenderer::Start()
{
    WeakPtr<SkinnedMeshRenderer> renderer{ weak_from_this() };
    s_renderingManager->AddRenderer(renderer.get());
    transform_ = GameObject()->Transform();
}

void Glib::SkinnedMeshRenderer::LateUpdate()
{
    if (!isEnabled_ || bones_.empty()) return;
    Matrix4x4 world = Matrix4x4::TRS(
        transform_->Position(),
        transform_->Rotation(),
        transform_->Scale()
    );

    auto buffer = s_dx12->GetConstantBuffer();

    worldConstantBuffer_ = buffer->Alloc(&world, sizeof(Matrix4x4));
    ComputeBone();
    boneConstantBuffer_ = buffer->Alloc(boneMatrix_.data(), sizeof(Matrix4x4) * boneMatrix_.size());
}

void Glib::SkinnedMeshRenderer::Draw(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!isEnabled_ || bones_.empty()) return;
    // パイプラインと各定数バッファを設定して描画
    s_graphics->SetPipelineState(ID::SKINNED_MESH_PIPELINESTATE);

    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(ID::SKINNED_MESH_WORLD_MATRIX, worldConstantBuffer_.Address());
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(ID::SKINNED_MESH_SKINNED_MATRIX, boneConstantBuffer_.Address());
    s_renderingManager->SetDirectionalLightConstant(ID::SKINNED_MESH_DIRECTIONAL_LIGHT);
    camera->SetConstantBuffer(ID::SKINNED_MESH_CAMERA_CONSTANT);
    camera->SetShadowMap(ID::SKINNED_MESH_SHADOW_MAP);
    s_meshManager->Draw(meshID_);
}

void Glib::SkinnedMeshRenderer::DrawShadow(const WeakPtr<Internal::CameraBase>& camera)
{
    s_graphics->SetPipelineState(ID::SKINNED_MESH_SHADOW_MAP);
    camera->SetConstantBuffer(0);
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(1, worldConstantBuffer_.Address());
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(2, boneConstantBuffer_.Address());
    s_meshManager->DrawShadow(meshID_);
}

unsigned int Glib::SkinnedMeshRenderer::MeshID() const
{
    return meshID_;
}

void Glib::SkinnedMeshRenderer::MeshID(unsigned int id)
{
    // メッシュとボーンを取得
    meshID_ = id;
    bones_ = s_meshManager->Bone(id);
    CreateBoneGameObject();
    isEnabled_ = !bones_.empty();
}

void Glib::SkinnedMeshRenderer::ComputeBone()
{
    auto boneSize = Mathf::Min(512, static_cast<int>(bones_.size()));
    for (int i = 1; i < boneSize; i++)
    {
        int parent = bones_.at(i).parent;
        if (parent == -1) continue;
        boneMatrix_.at(i) *= boneMatrix_.at(parent);
    }
}

void Glib::SkinnedMeshRenderer::CreateBoneGameObject()
{
    // ボーンがない場合は生成しない
    if (bones_.empty()) return;

    // すでにある場合は削除
    for (auto& bone : boneTransforms_)
    {
        bone->GameObject()->Destroy();
    }
    boneTransforms_.clear();

    // ボーンを生成
    boneTransforms_.resize(bones_.size());
    for (auto& bone : bones_)
    {
        // 親ボーンの設定とボーンのオブジェクト生成
        auto boneObject = GameObjectManager::Instantiate(bone.name);
        bone.parent != -1 ?
            boneObject->Transform()->Parent(boneTransforms_.at(bone.parent)) :
            boneObject->Transform()->Parent(GameObject()->Transform());

        // ボーントランスフォーム配列に追加
        auto& boneTransform = boneTransforms_.at(bone.boneIndex);
        boneTransform = boneObject->Transform();
        boneTransform->LocalScale(Vector3::One());
    }
}

const std::vector<Glib::Bone>& Glib::SkinnedMeshRenderer::Bones() const
{
    return bones_;
}

const std::vector<Glib::WeakPtr<Glib::Transform>>& Glib::SkinnedMeshRenderer::BoneTransforms() const
{
    return boneTransforms_;
}

std::array<Matrix4x4, 512>& Glib::SkinnedMeshRenderer::BoneMatrix()
{
    return boneMatrix_;
}

void Glib::SkinnedMeshRenderer::OnGUI()
{
    Component::OnGUI();
    int meshID = MeshID();
    if (GLGUI::InputInt("MeshID", &meshID))
    {
        MeshID(meshID);
    }
}
