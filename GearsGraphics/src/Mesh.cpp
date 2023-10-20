#include <Mesh.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>

using namespace  Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_resource = GraphicsResource::Instance();
}

bool Glib::Mesh::Load(std::string_view path)
{
    return false;
}

void Glib::Mesh::Draw()
{
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    vertexBuffer_.SetBuffer();
    indexBuffer_.SetBuffer();

    for (const auto& subset : subsets_)
    {
        const auto& material = materials_[subset.MaterialID];

        // アルベドテクスチャを設定
        material.albedo.expired() ?
            s_resource->SetTexture(ID::MAGENTA_TEXTURE, ID::MESH_ALBEDO) :
            material.albedo->SetTexture(ID::MESH_ALBEDO);

        // 法線マップを設定
        material.normal.expired() ?
            s_resource->SetTexture(ID::NORMAL_MAP_TEXTURE, ID::MESH_NORMAL_MAP) :
            material.normal->SetTexture(ID::MESH_NORMAL_MAP);

        // 定数バッファを設定
        material.params.SetBuffer(ID::MESH_MATERIAL_BUFFER);

        // 描画
        s_dx12->CommandList()->DrawIndexedInstanced(subset.indexEnd, 1, subset.indexStart, 0, 0);
    }
}

void Glib::Mesh::DrawShadow()
{
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    vertexBuffer_.SetBuffer();
    indexBuffer_.SetBuffer();

    for (const auto& subset : subsets_)
    {
        s_dx12->CommandList()->DrawIndexedInstanced(subset.indexEnd, 1, subset.indexStart, 0, 0);
    }
}
