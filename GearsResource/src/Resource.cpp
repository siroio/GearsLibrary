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

    /* �摜�ǂݍ��ݗp�֐��e�[�u�� */
    std::unordered_map<std::string, LoadFunction> s_loadFunctions;

    /* ���\�[�X�ꗗ */
    /* �e�N�X�`�� */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Texture>> s_textureResource;
    /* ���b�V�� */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Mesh>> s_meshResource;
    /* �A�j���[�V���� */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Animation>> s_animationResource;
}

bool Glib::Resource::Initialize()
{
    // ���[�h�֐��̒ǉ�
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
    //    // �A�b�v���[�h�p�o�b�t�@�쐬
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

    // ���b�V����ϊ�

    //for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    //{
    //    const aiMesh* aiMesh = scene->mMeshes[meshIndex];

    //    // �e���_���������ēƎ��`���̒��_�ɕϊ�
    //    for (unsigned int vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; ++vertexIndex)
    //    {
    //        Glib::MeshVertex vertex;
    //        // ���_���W�A�@���A�e�N�X�`�����W�A�ڐ��Ȃǂ𒊏o���Đݒ�
    //        // ...

    //        meshBuilder.AddVertex(vertex);
    //    }

    //    // �C���f�b�N�X���������ă��b�V���̎O�p�`���\�z
    //    for (unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; ++faceIndex)
    //    {
    //        const aiFace& face = aiMesh->mFaces[faceIndex];
    //        if (face.mNumIndices != 3)
    //        {
    //            // ��O�p�`�̃|���S���͖���
    //            continue;
    //        }

    //        // �C���f�b�N�X�����擾���Đݒ�
    //        meshBuilder.AddTriangle(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    //    }
    //}

    return nullptr;
}


Glib::WeakPtr<Glib::Animation> Glib::Resource::LoadAnim(std::string_view filePath)
{
    return nullptr;
}
