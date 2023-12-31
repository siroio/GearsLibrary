#include <Components/SkinnedMeshRenderer.h>
#include <Internal/RenderingManager.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <MeshManager.h>
#include <Mathf.h>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_renderingManager = RenderingManager::Instance();
    auto s_graphics = GraphicsResource::Instance();
    auto& s_meshManager = Glib::MeshManager::Instance();
}

Glib::SkinnedMeshRenderer::SkinnedMeshRenderer()
{
    worldConstantBuffer_.Create(sizeof(Matrix4x4));
    boneConstantBuffer_.Create(sizeof(Matrix4x4) * boneMatrix_.size());
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
    auto worldMatrix = Matrix4x4::TRS(
        transform_->Position(),
        transform_->Rotation(),
        transform_->Scale()
    );
    worldConstantBuffer_.Update(sizeof(Matrix4x4), &worldMatrix);
    ComputeBone();
    boneConstantBuffer_.Update(static_cast<UINT>(sizeof(Matrix4x4) * boneMatrix_.size()), boneMatrix_.data());
}

void Glib::SkinnedMeshRenderer::Draw(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!isEnabled_ || bones_.empty()) return;
    // �p�C�v���C���Ɗe�萔�o�b�t�@��ݒ肵�ĕ`��
    s_graphics->SetPipelineState(ID::SKINNED_MESH_PIPELINESTATE);
    worldConstantBuffer_.SetBuffer(ID::SKINNED_MESH_WORLD_MATRIX);
    boneConstantBuffer_.SetBuffer(ID::SKINNED_MESH_SKINNED_MATRIX);
    s_renderingManager->SetDirectionalLightConstant(ID::SKINNED_MESH_DIRECTIONAL_LIGHT);
    camera->SetConstantBuffer(ID::SKINNED_MESH_CAMERA_CONSTANT);
    camera->SetShadowMap(ID::SKINNED_MESH_SHADOW_MAP);
    s_meshManager.Draw(meshID_);
}

void Glib::SkinnedMeshRenderer::DrawShadow(const WeakPtr<Internal::CameraBase>& camera)
{
    s_graphics->SetPipelineState(ID::SKINNED_MESH_SHADOW_MAP);
    camera->SetConstantBuffer(0);
    worldConstantBuffer_.SetBuffer(1);
    boneConstantBuffer_.SetBuffer(2);
    s_meshManager.DrawShadow(meshID_);
}

unsigned int Glib::SkinnedMeshRenderer::MeshID() const
{
    return meshID_;
}

void Glib::SkinnedMeshRenderer::MeshID(unsigned int id)
{
    // ���b�V���ƃ{�[�����擾
    meshID_ = id;
    bones_ = s_meshManager.Bone(id);
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
    // �{�[�����Ȃ��ꍇ�͐������Ȃ�
    if (bones_.empty()) return;

    // ���łɂ���ꍇ�͍폜
    for (auto& bone : boneTransforms_)
    {
        bone->GameObject()->Destroy();
    }
    boneTransforms_.clear();

    // �{�[���𐶐�
    boneTransforms_.resize(bones_.size());
    for (auto& bone : bones_)
    {
        // �e�{�[���̐ݒ�ƃ{�[���̃I�u�W�F�N�g����
        auto boneObject = GameObjectManager::Instantiate(bone.name);
        bone.parent != -1 ?
            boneObject->Transform()->Parent(boneTransforms_.at(bone.parent)) :
            boneObject->Transform()->Parent(GameObject()->Transform());

        // �{�[���g�����X�t�H�[���z��ɒǉ�
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
