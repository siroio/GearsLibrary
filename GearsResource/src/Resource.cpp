#include <Resource.h>
#include <DirectXTex.h>
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
    using LoadFunction = std::function<HRESULT(std::wstring_view, DirectX::TexMetadata*, DirectX::ScratchImage&)>;

    /* 画像読み込み用関数テーブル */
    std::unordered_map<std::string, LoadFunction> s_loadFunctions;

    /* リソース一覧 */
    /* テクスチャ */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Texture>> s_textureResource;
    /* メッシュ */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Mesh>> s_meshResource;
    /* アニメーション */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Animation>> s_animationResource;
}

bool Glib::Resource::Initialize()
{
    // ロード関数の追加
    s_loadFunctions["tga"] = [](std::wstring_view path, DirectX::TexMetadata* meta, DirectX::ScratchImage& img)
    {
        return SUCCEEDED(DirectX::LoadFromTGAFile(path.data(), meta, img));
    };

    s_loadFunctions["dds"] = [](std::wstring_view path, DirectX::TexMetadata* meta, DirectX::ScratchImage& img)
    {
        return SUCCEEDED(DirectX::LoadFromDDSFile(path.data(), DirectX::DDS_FLAGS_NONE, meta, img));
    };

    s_loadFunctions["png"] = [](std::wstring_view path, DirectX::TexMetadata* meta, DirectX::ScratchImage& img)
    {
        return SUCCEEDED(DirectX::LoadFromWICFile(path.data(), DirectX::WIC_FLAGS_NONE, meta, img));
    };

    s_loadFunctions["bmp"] = s_loadFunctions["png"];
    s_loadFunctions["jpg"] = s_loadFunctions["png"];
    s_loadFunctions["jpeg"] = s_loadFunctions["png"];
    s_loadFunctions["spa"] = s_loadFunctions["png"];
    s_loadFunctions["sph"] = s_loadFunctions["png"];

    return true;
}

Glib::WeakPtr<Glib::Texture> Glib::Resource::LoadTex(std::string_view filePath)
{
    std::filesystem::path path = filePath;
    if (!path.has_extension()) return nullptr;
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};
    auto& func = s_loadFunctions.at(path.extension().string());
    auto result = func(path.wstring(), &metadata, scratchImg);
    auto img = scratchImg.GetImage(0, 0, 0);

    //{
    //    // アップロード用バッファ作成
    //    ComPtr<ID3D10Resource> uploadBuffer{ nullptr };
    //    UINT64 width = (static_cast<UINT64>(img->rowPitch) + ((256ULL - (img->rowPitch % 256)) % 256)) * static_cast<UINT64>(img->height);
    //}

    return nullptr;
}

Glib::WeakPtr<Glib::Mesh> Glib::Resource::LoadMesh(std::string_view filePath)
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


Glib::WeakPtr<Glib::Animation> Glib::Resource::LoadAnim(std::string_view filePath)
{
    return nullptr;
}
