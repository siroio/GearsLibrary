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
    /* ���\�[�X�ꗗ */

    /* ���b�V�� */
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Mesh>> s_meshResource;
    /* �A�j���[�V���� */
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


Glib::WeakPtr<Glib::Animation> Glib::Resource::LoadAnim(unsigned int id, std::string_view filePath)
{
    return nullptr;
}
