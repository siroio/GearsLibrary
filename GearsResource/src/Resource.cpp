#include <Resource.h>
#include <DirectXTex.h>
#include <Internal/DX12/DirectX12.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <StringUtility.h>
#include <ComPtr.h>
#include <unordered_map>
#include <filesystem>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

namespace
{
    /* リソース一覧 */

    /* メッシュ */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Mesh>> s_meshResource;
    /* アニメーション */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Animation>> s_animationResource;
}

Glib::WeakPtr<Glib::Mesh> Glib::Resource::LoadMesh(unsigned int id, std::string_view filePath)
{
    Assimp::Importer importer{};
    unsigned int flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;

    const aiScene* scene{ nullptr };
    scene = importer.ReadFile(filePath.data(), flag);
    if (scene) return nullptr;

    // メッシュを変換

    //for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    //{
    //    const aiMesh* aiMesh = scene->mMeshes[meshIndex];

    //    // 各頂点を処理して独自形式の頂点に変換
    //    for (unsigned int vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; ++vertexIndex)
    //    {
    //        Glib::MeshVertex vertex;
    //        // 頂点座標、法線、テクスチャ座標、接線などを抽出して設定
    //        // ...

    //        meshBuilder.AddVertex(vertex);
    //    }

    //    // インデックスを処理してメッシュの三角形を構築
    //    for (unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; ++faceIndex)
    //    {
    //        const aiFace& face = aiMesh->mFaces[faceIndex];
    //        if (face.mNumIndices != 3)
    //        {
    //            // 非三角形のポリゴンは無視
    //            continue;
    //        }

    //        // インデックス情報を取得して設定
    //        meshBuilder.AddTriangle(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    //    }
    //}

    return nullptr;
}


Glib::WeakPtr<Glib::Animation> Glib::Resource::LoadAnim(unsigned int id, std::string_view filePath)
{
    return nullptr;
}
