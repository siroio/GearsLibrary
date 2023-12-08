#include <Mesh.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <TextureManager.h>
#include <GLObject.h>
#include <Color.h>
#include <iterator>
#include <filesystem>

using namespace Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_resource = GraphicsResource::Instance();
    auto& s_textureManager = Glib::TextureManager::Instance();

    struct Vertex
    {
        float position[3];
        float normal[3];
        float uv[2];
        int boneIndex[4];
        float boneWeight[4];
        float tangent[4];
    };
}

bool Glib::Mesh::Load(std::string_view path)
{
    GLObject object{};
    if (!object.ReadFile(path)) return false;
    std::filesystem::path folderPath = path;
    vertexBuffer_.Create(sizeof(Vertex), static_cast<unsigned int>(object.Vertices().size()));
    vertexBuffer_.Update(object.Vertices().data());
    indexBuffer_.Create(sizeof(unsigned int), static_cast<unsigned int>(object.Indices().size()));
    indexBuffer_.Update(object.Indices().data());

    for (const auto& subset : object.Subsets())
    {
        subsets_.emplace_back(subset.indexStart, subset.indexCount, subset.material);
    }

    const auto paramSize = sizeof(Color) * 3 + sizeof(float);
    const auto& materials = object.Materials();
    materials_.resize(materials.size());
    for (int i = 0; i < materials.size(); i++)
    {
        if (!materials_[i].params.Create(paramSize)) return false;
        materials_[i].params.Update(paramSize, &materials[i]);
        if (!materials[i].texture.empty())
        {
            auto texPath = folderPath.remove_filename().string() + materials[i].texture;
            materials_[i].albedo = s_textureManager.Load(texPath);
        }
        if (!materials[i].normal.empty())
        {
            auto normalPath = folderPath.remove_filename().string() + materials[i].normal;
            materials_[i].normal = s_textureManager.Load(normalPath);
        }
    }

    auto boneSize = object.Bones().size();
    for (int i = 0; i < boneSize; i++)
    {
        const auto& bone = object.Bones().at(i);
        Glib::Bone newBone{};
        newBone.name = std::string{ bone.boneName };
        newBone.position = Vector3{ bone.translate[0], bone.translate[1], bone.translate[2] };
        newBone.parent = bone.parent;
        newBone.boneIndex = i;
        bones_.emplace_back(newBone);
    }

    return true;
}

void Glib::Mesh::Draw()
{
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    vertexBuffer_.SetBuffer();
    indexBuffer_.SetBuffer();

    for (const auto& subset : subsets_)
    {
        const auto& material = materials_.at(subset.materialID);

        //アルベドテクスチャを設定
        if (material.albedo.expired())
        {
            s_resource->SetTexture(ID::WHITE_TEXTURE, ID::MESH_ALBEDO);
        }
        else
        {
            material.albedo->SetTexture(ID::MESH_ALBEDO);
        }


        // 法線マップを設定
        if (material.normal.expired())
        {
            s_resource->SetTexture(ID::NORMAL_MAP_TEXTURE, ID::MESH_NORMAL_MAP);
        }
        else
        {
            material.normal->SetTexture(ID::MESH_NORMAL_MAP);
        }

        // 定数バッファを設定
        material.params.SetBuffer(ID::MESH_MATERIAL_BUFFER);

        // 描画
        s_dx12->CommandList()->DrawIndexedInstanced(subset.indexCount, 1, subset.indexStart, 0, 0);
    }
}

void Glib::Mesh::DrawShadow()
{
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    vertexBuffer_.SetBuffer();
    indexBuffer_.SetBuffer();

    for (const auto& subset : subsets_)
    {
        s_dx12->CommandList()->DrawIndexedInstanced(subset.indexCount, 1, subset.indexStart, 0, 0);
    }
}

const std::vector<Glib::Bone>& Glib::Mesh::Bone() const
{
    return bones_;
}
