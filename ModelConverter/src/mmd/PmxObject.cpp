#include <mmd/PmxObject.h>
#include <filesystem>
#include <fstream>
#include <array>
#include <GLObject.h>
#include <utility/FileUtility.h>
#include <external/mikktspace.h>
#include <iostream>

namespace fs = std::filesystem;

namespace
{
    static constexpr std::array<unsigned char, 4> PMX_MAGIC_NUMBER{ 0x50, 0x4d, 0x58, 0x20 };
    static constexpr int PMX_NO_DATA = -1;
    static constexpr float PMX_VERSION = 2.0f;
}

namespace
{
    bool CheckHasBoneFlag(const PmxBone& bone, const PmxBoneFlag& flag)
    {
        return ((unsigned int)bone.flag & (unsigned int)flag) != 0;
    }

    Encode ToEncode(PmxEncode encode)
    {
        switch (encode)
        {
            case PmxEncode::UTF16:
                return Encode::UTF16;
            case PmxEncode::UTF8:
                return Encode::UTF8;
            default:
                return Encode::UTF8;
        }
    }
}

bool PmxModel::ReadPmxHeader(std::ifstream& pmxFile, PmxHeader& header)
{
    // マジックナンバー読み込み
    std::array<unsigned char, 4> magicNum{};
    FileUtility::ReadForBinary(pmxFile, magicNum.data(), 4);
    if (magicNum != PMX_MAGIC_NUMBER) return false;

    // バージョン2.0のチェック
    FileUtility::ReadForBinary(pmxFile, &header.version, 4);
    if (header.version != PMX_VERSION) return false;

    // 追加情報の長さ
    FileUtility::ReadForBinary(pmxFile, &header.length, 1);
    FileUtility::ReadForBinary(pmxFile, &header.encode, 1);

    for (int i = 0; i < header.length - 1; i++)
    {
        header.Info.push_back(pmxFile.get());
    }

    // モデル名と説明はスキップ
    for (int i = 0; i < 4; i++)
    {
        int len{ 0 };
        FileUtility::ReadForBinary(pmxFile, &len, sizeof(int));
        pmxFile.seekg(len, std::ios::cur);
    }

    return true;
}

bool PmxModel::ReadVertices(std::ifstream& pmxFile, const PmxHeader& header)
{
    // 頂点
    int vertexSize{ 0 };
    FileUtility::ReadForBinary(pmxFile, &vertexSize, 4);

    vertices.resize(vertexSize);
    for (int i = 0; i < vertexSize; i++)
    {
        auto& vertex = vertices.at(i);
        FileUtility::ReadForBinary(pmxFile, &vertex.position, sizeof(Vector3f));
        FileUtility::ReadForBinary(pmxFile, &vertex.normal, sizeof(Vector3f));
        FileUtility::ReadForBinary(pmxFile, &vertex.uv, sizeof(Vector2f));

        // 追加UV
        auto uvCount = header.Info[0];
        if (uvCount != 0)
        {
            vertex.additionalUV.resize(uvCount);
            for (int uv = 0; uv < uvCount; uv++)
            {
                pmxFile.read(reinterpret_cast<char*>(&vertex.additionalUV[uv]), 16);
            }
        }

        // ウェイトの種類
        const auto weightType = static_cast<WeightType>(pmxFile.get());

        switch (weightType)
        {
            case WeightType::BDEF1:
                vertex.weight.type = WeightType::BDEF1;
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.bone1, header.Info[4]);
                vertex.weight.bone2 = PMX_NO_DATA;
                vertex.weight.bone3 = PMX_NO_DATA;
                vertex.weight.bone4 = PMX_NO_DATA;
                vertex.weight.weight1 = 1.0f;
                break;
            case WeightType::BDEF2:
                vertex.weight.type = WeightType::BDEF2;
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.bone1, header.Info[4]);
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.bone2, header.Info[4]);
                vertex.weight.bone3 = PMX_NO_DATA;
                vertex.weight.bone4 = PMX_NO_DATA;
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.weight1, sizeof(float));
                vertex.weight.weight2 = 1.0f - vertex.weight.weight1;
                break;
            case WeightType::BDEF4:
                vertex.weight.type = WeightType::BDEF4;
                for (int i = 0; i < 4; i++)
                {
                    FileUtility::ReadForBinary(pmxFile, &vertex.weight.bones[i], header.Info[4]);
                }
                for (int i = 0; i < 4; i++)
                {
                    FileUtility::ReadForBinary(pmxFile, &vertex.weight.weights[i], header.Info[4]);
                }
                break;
            case WeightType::SDEF:
                vertex.weight.type = WeightType::SDEF;
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.bone1, header.Info[4]);
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.bone2, header.Info[4]);
                vertex.weight.bone3 = PMX_NO_DATA;
                vertex.weight.bone4 = PMX_NO_DATA;
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.weight1, sizeof(float));
                vertex.weight.weight2 = 1.0f - vertex.weight.weight1;
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.c, sizeof(Vector3f));
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.r0, sizeof(Vector3f));
                FileUtility::ReadForBinary(pmxFile, &vertex.weight.r1, sizeof(Vector3f));
                break;
            default:
                return false;
        }

        // エッジ
        pmxFile.seekg(4, std::ios::cur);
        if (vertex.weight.bone1 == PMX_NO_DATA) return false;
    }
    return true;
}

bool PmxModel::ReadSurfaces(std::ifstream& pmxFile, const PmxHeader& header)
{
    // 頂点インデックス
    int surfaceSize{ 0 };
    FileUtility::ReadForBinary(pmxFile, &surfaceSize, sizeof(int));

    surfaces.resize(surfaceSize);
    for (int i = 0; i < surfaceSize; i++)
    {
        auto& surface = surfaces.at(i);
        FileUtility::ReadForBinary(pmxFile, &surface.vertexIndex, header.Info[1]);
        if (surface.vertexIndex == PMX_NO_DATA) return false;
    }
    return true;
}

bool PmxModel::ReadTextures(std::ifstream& pmxFile, const PmxHeader& header)
{
    // テクスチャ
    int textureSize{ 0 };
    FileUtility::ReadForBinary(pmxFile, &textureSize, sizeof(int));

    texturePath.resize(textureSize);
    for (int i = 0; i < textureSize; i++)
    {
        std::string textPath{ "" };
        FileUtility::ReadText(pmxFile, textPath, ToEncode(header.encode));
        textPath = fs::path{ textPath }.make_preferred().lexically_normal().generic_string();
        texturePath.at(i) = textPath;
    }
    return true;
}

bool PmxModel::ReadMaterials(std::ifstream& pmxFile, const PmxHeader& header)
{
    // マテリアル
    int materialSize{ 0 };
    FileUtility::ReadForBinary(pmxFile, &materialSize, sizeof(4));
    materials.resize(materialSize);

    for (int i = 0; i < materialSize; i++)
    {
        // 素材名 & 素材名(英名)
        for (int i = 0; i < 2; i++)
        {
            int len{ 0 };
            FileUtility::ReadForBinary(pmxFile, &len, sizeof(int));
            pmxFile.seekg(len, std::ios::cur);
        }
        auto& material = materials.at(i);
        FileUtility::ReadForBinary(pmxFile, &material.diffuse, sizeof(Vector4f));
        FileUtility::ReadForBinary(pmxFile, &material.specular, sizeof(Vector3f));
        FileUtility::ReadForBinary(pmxFile, &material.specularFactor, sizeof(float));
        FileUtility::ReadForBinary(pmxFile, &material.ambient, sizeof(Vector3f));

        // フラグ
        pmxFile.get();

        // エッジ色
        pmxFile.seekg(16, std::ios::cur);
        // エッジサイズ
        pmxFile.seekg(4, std::ios::cur);

        // テクスチャ
        FileUtility::ReadForBinary(pmxFile, &material.textureIndex, header.Info[2]);
        pmxFile.seekg(header.Info[2], std::ios::cur); // スフィアは飛ばす
        pmxFile.get();

        const unsigned char sharedToonFlag = pmxFile.get();
        if (sharedToonFlag)
        {
            pmxFile.seekg(1, std::ios::cur);
        }
        else
        {
            FileUtility::ReadForBinary(pmxFile, &material.toonTextureIndex, header.Info[2]);
        }

        int memoLen{ 0 };
        FileUtility::ReadForBinary(pmxFile, &memoLen, sizeof(int));
        pmxFile.seekg(memoLen, std::ios::cur);
        FileUtility::ReadForBinary(pmxFile, &material.vertexNum, sizeof(int));
    }
    return true;
}

bool PmxModel::ReadBones(std::ifstream& pmxFile, const PmxHeader& header)
{
    // ボーン
    int boneSize{ 0 };
    int ikLinkSize{ 0 };
    unsigned char angleLimit = 0;
    FileUtility::ReadForBinary(pmxFile, &boneSize, sizeof(int));

    bones.resize(boneSize);
    for (int i = 0; i < boneSize; i++)
    {
        auto& bone = bones.at(i);
        // ボーン名
        FileUtility::ReadText(pmxFile, bone.name, ToEncode(header.encode));
        FileUtility::ReadText(pmxFile, bone.nameBoneEng, Encode::UTF8);

        FileUtility::ReadForBinary(pmxFile, &bone.position, sizeof(Vector3f));
        FileUtility::ReadForBinary(pmxFile, &bone.parentBoneIndex, header.Info[4]);
        if (boneSize <= bone.parentBoneIndex)
        {
            bone.parentBoneIndex = PMX_NO_DATA;
        }
        FileUtility::ReadForBinary(pmxFile, &bone.transformHierarchy, sizeof(int));
        FileUtility::ReadForBinary(pmxFile, &bone.flag, 2);

        // 各フラグのチェック
        if (CheckHasBoneFlag(bone, PmxBoneFlag::TargetShowMode))
        {
            FileUtility::ReadForBinary(pmxFile, &bone.childrenIndex, header.Info[4]);
            if (boneSize <= bone.childrenIndex)
            {
                bone.childrenIndex = PMX_NO_DATA;
            }
        }
        else
        {
            bone.childrenIndex = PMX_NO_DATA;
            FileUtility::ReadForBinary(pmxFile, &bone.offset, sizeof(Vector3f));
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::AppendRotate) || CheckHasBoneFlag(bone, PmxBoneFlag::AppendTranslate))
        {
            FileUtility::ReadForBinary(pmxFile, &bone.impartParentIndex, header.Info[4]);
            FileUtility::ReadForBinary(pmxFile, &bone.impartRate, sizeof(float));
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::FixedAxis))
        {
            FileUtility::ReadForBinary(pmxFile, &bone.fixedAxis, sizeof(Vector3f));
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::LocalAxis))
        {
            FileUtility::ReadForBinary(pmxFile, &bone.localAxisX, sizeof(Vector3f));
            FileUtility::ReadForBinary(pmxFile, &bone.localAxisZ, sizeof(Vector3f));
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::DeformOuterParent))
        {
            FileUtility::ReadForBinary(pmxFile, &bone.externalParentKey, sizeof(int));
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::IK))
        {
            FileUtility::ReadForBinary(pmxFile, &bone.ikTargetIndex, header.Info[4]);
            FileUtility::ReadForBinary(pmxFile, &bone.ikLoopCount, sizeof(int));
            FileUtility::ReadForBinary(pmxFile, &bone.ikUnitAngle, sizeof(float));
            FileUtility::ReadForBinary(pmxFile, &ikLinkSize, sizeof(int));

            bone.ikLinks.resize(ikLinkSize);
            for (int j = 0; j < ikLinkSize; ++j)
            {
                FileUtility::ReadForBinary(pmxFile, &bone.ikLinks[j].index, header.Info[4]);
                angleLimit = pmxFile.get();
                bone.ikLinks[j].existAngleLimited = false;
                if (angleLimit == 1)
                {
                    FileUtility::ReadForBinary(pmxFile, &bone.ikLinks[j].limitAngleMin, sizeof(Vector3f));
                    FileUtility::ReadForBinary(pmxFile, &bone.ikLinks[j].limitAngleMax, sizeof(Vector3f));
                    bone.ikLinks[j].existAngleLimited = true;
                }
            }
        }
        else
        {
            bone.ikTargetIndex = PMX_NO_DATA;
        }
    }
    return true;
}

const std::vector<PmxVertex>& PmxModel::Vertices()
{
    return vertices;
}

const std::vector<PmxSurface>& PmxModel::Surfaces()
{
    return surfaces;
}

const std::vector<std::string>& PmxModel::TexturePath()
{
    return texturePath;
}

const std::vector<PmxMaterial>& PmxModel::Materials()
{
    return materials;
}

const std::vector<PmxBone>& PmxModel::Bones()
{
    return bones;
}

bool PmxModel::LoadFile(std::string_view path)
{
    // 拡張子が間違っていたら失敗
    if (!FileUtility::GetExtension(path).ends_with("pmx")) return false;
    std::ifstream pmxFile{ path.data(), std::ios::binary };
    if (pmxFile.fail()) return false;

    PmxHeader header{};
    if (!ReadPmxHeader(pmxFile, header)) return false;
    if (!ReadVertices(pmxFile, header)) return false;
    if (!ReadSurfaces(pmxFile, header)) return false;
    if (!ReadTextures(pmxFile, header)) return false;
    if (!ReadMaterials(pmxFile, header)) return false;
    if (!ReadBones(pmxFile, header)) return false;
    return true;
}

bool PmxModel::WriteFile(std::string_view path)
{
    using gl = Glib::GLObject;
    std::vector<gl::Vertex> glVertices;
    std::vector<unsigned int> glIndices;
    std::vector<gl::Subset> glSubsets;
    std::vector<gl::Material> glMaterials;
    std::vector<gl::Bone> glBones;
    ComputeTangents();

    for (const auto& pmxVertex : vertices)
    {
        gl::Vertex glVertex{};
        // 頂点座標
        glVertex.position[0] = pmxVertex.position.x;
        glVertex.position[1] = pmxVertex.position.y;
        glVertex.position[2] = pmxVertex.position.z;
        // 法線
        glVertex.normal[0] = pmxVertex.normal.x;
        glVertex.normal[1] = pmxVertex.normal.y;
        glVertex.normal[2] = pmxVertex.normal.z;
        // UV座標
        glVertex.uv[0] = pmxVertex.uv.x;
        glVertex.uv[1] = pmxVertex.uv.y;

        // tangent
        glVertex.tangent[0] = pmxVertex.tangent.x;
        glVertex.tangent[1] = pmxVertex.tangent.y;
        glVertex.tangent[2] = pmxVertex.tangent.z;
        glVertex.tangent[3] = pmxVertex.tangent.w;

        // ボーン情報
        for (size_t i = 0; i < 4; ++i)
        {
            glVertex.boneIndex[i] = pmxVertex.weight.bones[i];
            glVertex.boneWeight[i] = pmxVertex.weight.weights[i];
        }

        glVertices.push_back(glVertex);
    }

    for (auto& pmxIndex : surfaces)
    {
        glIndices.push_back(pmxIndex.vertexIndex);
    }

    int subsetNum = static_cast<int>(materials.size());
    int startIndex{ 0 };
    for (int i = 0; i < subsetNum; i++)
    {
        auto& mat = materials.at(i);
        gl::Subset subset{};
        subset.indexStart = startIndex;
        subset.indexCount = mat.vertexNum;
        subset.material = i;
        startIndex += mat.vertexNum;
        glSubsets.push_back(subset);
    }

    for (auto& mat : materials)
    {
        gl::Material glMat{};
        glMat.ambient[0] = mat.ambient.x;
        glMat.ambient[1] = mat.ambient.y;
        glMat.ambient[2] = mat.ambient.z;
        glMat.ambient[3] = 1.0f;
        glMat.diffuse[0] = mat.diffuse.x;
        glMat.diffuse[1] = mat.diffuse.y;
        glMat.diffuse[2] = mat.diffuse.z;
        glMat.diffuse[3] = mat.diffuse.w;
        glMat.specular[0] = mat.specular.x;
        glMat.specular[1] = mat.specular.y;
        glMat.specular[2] = mat.specular.z;
        glMat.specular[3] = 1.0f;
        glMat.shininess = mat.specularFactor;
        if (mat.textureIndex != 0xFF)
        {
            glMat.texture = texturePath.at(mat.textureIndex);
        }
        glMaterials.push_back(glMat);
    }

    for (auto& bone : bones)
    {
        gl::Bone glBone{};
        glBone.boneName = bone.name;
        glBone.parent = bone.parentBoneIndex;
        glBone.translate[0] = bone.position.x;
        glBone.translate[1] = bone.position.y;
        glBone.translate[2] = bone.position.z;
        glBones.push_back(glBone);
    }
    auto mesh = Glib::GLObject{ glVertices, glIndices, glSubsets, glMaterials, glBones };
    return mesh.WriteFile(std::string{ path } + ".globj");
}

void PmxModel::ComputeTangents()
{
    constexpr int TRIANGLE_VERTEX_COUNT{ 3 };
    SMikkTSpaceInterface mikkInterface{};
    mikkInterface.m_getNumFaces = [](const SMikkTSpaceContext* pContext)
    {
        auto model = static_cast<PmxModel*>(pContext->m_pUserData);
        return static_cast<int>(model->Surfaces().size() / TRIANGLE_VERTEX_COUNT);
    };
    mikkInterface.m_getNumVerticesOfFace = [](const SMikkTSpaceContext* pContext, int iFace)
    {
        return TRIANGLE_VERTEX_COUNT;
    };
    mikkInterface.m_getPosition = [](const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert)
    {
        const auto& model = static_cast<PmxModel*>(pContext->m_pUserData);
        const auto vertexIndex = model->Surfaces()[(static_cast<size_t>(iFace) * TRIANGLE_VERTEX_COUNT) + iVert].vertexIndex;
        memcpy(fvPosOut, model->Vertices()[vertexIndex].position.elem, sizeof(float) * 3);
    };
    mikkInterface.m_getNormal = [](const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert)
    {
        const auto& model = static_cast<PmxModel*>(pContext->m_pUserData);
        const auto vertexIndex = model->Surfaces()[(static_cast<size_t>(iFace) * TRIANGLE_VERTEX_COUNT) + iVert].vertexIndex;
        memcpy(fvPosOut, model->Vertices()[vertexIndex].normal.elem, sizeof(float) * 3);
    };
    mikkInterface.m_getTexCoord = [](const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert)
    {
        const auto& model = static_cast<PmxModel*>(pContext->m_pUserData);
        const auto vertexIndex = model->Surfaces()[(static_cast<size_t>(iFace) * TRIANGLE_VERTEX_COUNT) + iVert].vertexIndex;
        memcpy(fvPosOut, model->Vertices()[vertexIndex].uv.elem, sizeof(float) * 2);
    };
    mikkInterface.m_setTSpaceBasic = [](const SMikkTSpaceContext* pContext, const float fvTangent[], const float fSign, const int iFace, const int iVert)
    {
        const auto& model = static_cast<PmxModel*>(pContext->m_pUserData);
        const auto vertexIndex = model->Surfaces()[(static_cast<size_t>(iFace) * TRIANGLE_VERTEX_COUNT) + iVert].vertexIndex;
        auto& vertices = const_cast<std::vector<PmxVertex>&>(model->Vertices());
        vertices[vertexIndex].tangent.x = fvTangent[0];
        vertices[vertexIndex].tangent.y = fvTangent[1];
        vertices[vertexIndex].tangent.z = fvTangent[2];
        vertices[vertexIndex].tangent.w = fSign;
    };

    SMikkTSpaceContext mikkContext{};
    mikkContext.m_pUserData = this;
    mikkContext.m_pInterface = &mikkInterface;
    if (genTangSpaceDefault(&mikkContext) == 0)
    {
        throw std::exception("Failed to generate tangents");
    }
}
