#include <mmd/PmxObject.h>
#include <filesystem>
#include <fstream>
#include <array>
#include <Windows.h>
#include <GLObject.h>


namespace fs = std::filesystem;

namespace
{
    // 拡張子を取得
    std::string GetExt(std::string_view file)
    {
        return fs::path{ file }.extension().string();
    }

    // フォルダを取得
    std::string GetFolder(std::string_view file)
    {
        return fs::path{ file }.relative_path().string();
    }

    std::string ReadTextBuf(std::ifstream& file, EncodeType encode)
    {
        int bufLength{ 0 };
        file.read(reinterpret_cast<char*>(&bufLength), 4);
        if (bufLength < 1) return "";
        if (encode == EncodeType::UTF16)
        {
            std::wstring textBuf(bufLength / sizeof(wchar_t), L'\0');
            file.read(reinterpret_cast<char*>(textBuf.data()), bufLength);

            int bufferSize = WideCharToMultiByte(CP_ACP, 0, textBuf.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (bufferSize == 0) return "";
            std::string result(bufferSize, '\0');
            if (WideCharToMultiByte(CP_ACP, 0, textBuf.data(), -1, result.data(), bufferSize, nullptr, nullptr) == 0)
            {
                return "";
            }

            return result;
        }
        else if (encode == EncodeType::UTF8)
        {
            std::string textBuf(bufLength, '\0');
            file.read(textBuf.data(), bufLength);
            return textBuf;
        }
        return "";
    }

    bool CheckHasBoneFlag(const PmxBone& bone, const PmxBoneFlag& flag)
    {
        return ((unsigned int)bone.flag & (unsigned int)flag) != 0;
    }

    static constexpr std::array<unsigned char, 4> PMX_MAGIC_NUMBER{ 0x50, 0x4d, 0x58, 0x20 };
    static constexpr int PMX_NO_DATA = -1;
}

bool PmxModel::LoadModel(std::string_view path)
{
    // 拡張子が間違っていたら失敗
    if (GetExt(path) != ".pmx") return false;
    std::ifstream pmxFile{ path.data(), std::ios::binary };
    if (pmxFile.fail()) return false;

    // ヘッダ
    PmxHeader header{};

    // マジックナンバー読み込み
    std::array<unsigned char, 4> magicNum{};
    pmxFile.read(reinterpret_cast<char*>(magicNum.data()), 4);
    if (magicNum != PMX_MAGIC_NUMBER) return false;

    // バージョン2.0のチェック
    pmxFile.read(reinterpret_cast<char*>(&header.version), 4);
    if (header.version != 2.0f) return false;

    // 追加情報の長さ
    pmxFile.read(reinterpret_cast<char*>(&header.length), 1);
    header.encode = static_cast<EncodeType>(pmxFile.get());

    for (int i = 0; i < header.length - 1; i++)
    {
        header.Info.push_back(pmxFile.get());
    }

    // モデル名と説明はスキップ
    for (int i = 0; i < 4; i++)
    {
        int len{ 0 };
        pmxFile.read(reinterpret_cast<char*>(&len), 4);
        pmxFile.seekg(len, std::ios::cur);
    }

    //頂点の読み込み
    int vertexSize{ 0 };
    pmxFile.read(reinterpret_cast<char*>(&vertexSize), 4);
    vertices.resize(vertexSize);
    for (int i = 0; i < vertexSize; i++)
    {
        auto& vertex = vertices.at(i);
        pmxFile.read(reinterpret_cast<char*>(&vertex.position), 12);
        pmxFile.read(reinterpret_cast<char*>(&vertex.normal), 12);
        pmxFile.read(reinterpret_cast<char*>(&vertex.uv), 8);
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
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.bone1), header.Info[4]);
                vertex.weight.bone2 = PMX_NO_DATA;
                vertex.weight.bone3 = PMX_NO_DATA;
                vertex.weight.bone4 = PMX_NO_DATA;
                vertex.weight.weight1 = 1.0f;
                break;
            case WeightType::BDEF2:
                vertex.weight.type = WeightType::BDEF2;
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.bone1), header.Info[4]);
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.bone2), header.Info[4]);
                vertex.weight.bone3 = PMX_NO_DATA;
                vertex.weight.bone4 = PMX_NO_DATA;
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.weight1), 4);
                vertex.weight.weight2 = 1.0f - vertex.weight.weight1;
                break;
            case WeightType::BDEF4:
                vertex.weight.type = WeightType::BDEF4;
                for (int i = 0; i < 4; i++)
                {
                    pmxFile.read(reinterpret_cast<char*>(&vertex.weight.bones[i]), header.Info[4]);
                }
                for (int i = 0; i < 4; i++)
                {
                    pmxFile.read(reinterpret_cast<char*>(&vertex.weight.weights[i]), 4);
                }
                break;
            case WeightType::SDEF:
                vertex.weight.type = WeightType::SDEF;
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.bone1), header.Info[4]);
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.bone2), header.Info[4]);
                vertex.weight.bone3 = PMX_NO_DATA;
                vertex.weight.bone4 = PMX_NO_DATA;
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.weight1), 4);
                vertex.weight.weight2 = 1.0f - vertex.weight.weight1;
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.c), 12);
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.r0), 12);
                pmxFile.read(reinterpret_cast<char*>(&vertex.weight.r1), 12);
                break;
            default:
                return false;
        }

        // エッジ
        pmxFile.seekg(4, std::ios::cur);
        if (vertex.weight.bone1 == PMX_NO_DATA) return false;
    }

    // 頂点インデックス
    int surfaceSize{ 0 };
    pmxFile.read(reinterpret_cast<char*>(&surfaceSize), 4);
    surfaces.resize(surfaceSize);

    for (int i = 0; i < surfaceSize; i++)
    {
        auto& surface = surfaces.at(i);
        pmxFile.read(reinterpret_cast<char*>(&surface.vertexIndex), header.Info[1]);
        if (surface.vertexIndex == PMX_NO_DATA) return false;
    }

    // テクスチャ
    int textureSize{ 0 };
    pmxFile.read(reinterpret_cast<char*>(&textureSize), 4);
    texturePath.resize(textureSize);
    for (int i = 0; i < textureSize; i++)
    {
        auto texPath = ReadTextBuf(pmxFile, header.encode);
        texPath = fs::path{ texPath }.make_preferred().lexically_normal().string();
        texturePath.at(i) = texPath;
    }

    // マテリアル
    int materialSize{ 0 };
    pmxFile.read(reinterpret_cast<char*>(&materialSize), 4);
    materials.resize(materialSize);
    for (int i = 0; i < materialSize; i++)
    {
        // 素材名 & 素材名(英名)
        for (int i = 0; i < 2; i++)
        {
            int len{ 0 };
            pmxFile.read(reinterpret_cast<char*>(&len), 4);
            pmxFile.seekg(len, std::ios::cur);
        }
        auto& material = materials.at(i);
        pmxFile.read(reinterpret_cast<char*>(&material.diffuse), 16);
        pmxFile.read(reinterpret_cast<char*>(&material.specular), 12);
        pmxFile.read(reinterpret_cast<char*>(&material.specularFactor), 4);
        pmxFile.read(reinterpret_cast<char*>(&material.ambient), 12);

        // フラグ
        pmxFile.get();

        // エッジ色
        pmxFile.seekg(16, std::ios::cur);
        // エッジサイズ
        pmxFile.seekg(4, std::ios::cur);

        // テクスチャ
        pmxFile.read(reinterpret_cast<char*>(&material.textureIndex), header.Info[2]);
        pmxFile.seekg(header.Info[2], std::ios::cur); // スフィアは飛ばす
        pmxFile.get();

        const unsigned char sharedToonFlag = pmxFile.get();
        sharedToonFlag ?
            pmxFile.seekg(1, std::ios::cur) :
            pmxFile.read(reinterpret_cast<char*>(&material.toonTextureIndex), header.Info[2]);

        int memoLen{ 0 };
        pmxFile.read(reinterpret_cast<char*>(&memoLen), 4);
        pmxFile.seekg(memoLen, std::ios::cur);

        pmxFile.read(reinterpret_cast<char*>(&material.vertexNum), 4);
    }

    int boneSize{ 0 };
    int ikLinkSize{ 0 };
    unsigned char angleLimit = 0;
    pmxFile.read(reinterpret_cast<char*>(&boneSize), 4);
    bones.resize(boneSize);

    for (int i = 0; i < boneSize; i++)
    {
        auto& bone = bones.at(i);
        // ボーン名
        bone.name = ReadTextBuf(pmxFile, header.encode);
        bone.nameBoneEng = ReadTextBuf(pmxFile, EncodeType::UTF8);

        pmxFile.read(reinterpret_cast<char*>(&bone.position), 12);
        pmxFile.read(reinterpret_cast<char*>(&bone.parentBoneIndex), header.Info[4]);
        if (boneSize <= bone.parentBoneIndex)
        {
            bone.parentBoneIndex = PMX_NO_DATA;
        }
        pmxFile.read(reinterpret_cast<char*>(&bone.transformHierarchy), 4);
        pmxFile.read(reinterpret_cast<char*>(&bone.flag), 2);

        if (CheckHasBoneFlag(bone, PmxBoneFlag::TargetShowMode))
        {
            pmxFile.read(reinterpret_cast<char*>(&bone.childrenIndex), header.Info[4]);
            if (boneSize <= bone.childrenIndex)
            {
                bone.childrenIndex = PMX_NO_DATA;
            }
        }
        else
        {
            bone.childrenIndex = PMX_NO_DATA;
            pmxFile.read(reinterpret_cast<char*>(&bone.offset), 12);
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::AppendRotate) || CheckHasBoneFlag(bone, PmxBoneFlag::AppendTranslate))
        {
            pmxFile.read(reinterpret_cast<char*>(&bone.impartParentIndex), header.Info[4]);
            pmxFile.read(reinterpret_cast<char*>(&bone.impartRate), 4);
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::FixedAxis))
        {
            pmxFile.read(reinterpret_cast<char*>(&bone.fixedAxis), 12);
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::LocalAxis))
        {
            pmxFile.read(reinterpret_cast<char*>(&bone.localAxisX), 12);
            pmxFile.read(reinterpret_cast<char*>(&bone.localAxisZ), 12);
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::DeformOuterParent))
        {
            pmxFile.read(reinterpret_cast<char*>(&bone.externalParentKey), 4);
        }

        if (CheckHasBoneFlag(bone, PmxBoneFlag::IK))
        {
            pmxFile.read(reinterpret_cast<char*>(&bone.ikTargetIndex), header.Info[4]);
            pmxFile.read(reinterpret_cast<char*>(&bone.ikLoopCount), 4);
            pmxFile.read(reinterpret_cast<char*>(&bone.ikUnitAngle), 4);
            pmxFile.read(reinterpret_cast<char*>(&ikLinkSize), 4);
            bone.ikLinks.resize(ikLinkSize);

            for (int j = 0; j < ikLinkSize; ++j)
            {
                pmxFile.read(reinterpret_cast<char*>(&bone.ikLinks[j].index), header.Info[4]);
                angleLimit = pmxFile.get();
                bone.ikLinks[j].existAngleLimited = false;
                if (angleLimit == 1)
                {
                    pmxFile.read(reinterpret_cast<char*>(&bone.ikLinks[j].limitAngleMin), 12);
                    pmxFile.read(reinterpret_cast<char*>(&bone.ikLinks[j].limitAngleMax), 12);
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

bool PmxModel::WriteModel(std::string_view path)
{
    std::vector<Glib::GLObject::Vertex> glVertices;
    std::vector<unsigned int> glIndices;
    std::vector<Glib::GLObject::Subset> glSubsets;
    std::vector<Glib::GLObject::Material> glMaterials;
    std::vector<Glib::GLObject::Bone> glBones;

    using gl = Glib::GLObject;
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

        // tangent計算
        Vector3f tangent{};
        tangent = Vector3f::Cross(pmxVertex.normal, { 0, 1, 0 });
        if (tangent == Vector3f{ 0.0f, 0.0f, 0.0f })
        {
            tangent = { 1.0f, 0.0f, 0.0f };
        }
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
        subset.indecCount = mat.vertexNum;
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
        glMat.ambient[3] = 1;
        glMat.diffuse[0] = mat.diffuse.x;
        glMat.diffuse[1] = mat.diffuse.y;
        glMat.diffuse[2] = mat.diffuse.z;
        glMat.diffuse[3] = mat.diffuse.w;
        glMat.specular[0] = mat.specular.x;
        glMat.specular[1] = mat.specular.y;
        glMat.specular[2] = mat.specular.z;
        glMat.specular[3] = 1;
        glMat.shininess = mat.specularFactor;
        memset(glMat.texture, 0, sizeof(glMat.texture));
        if (mat.textureIndex != 0xFF)
        {
            std::string albedo = texturePath.at(mat.textureIndex);
            for (int i = 0; i < albedo.size(); i++)
            {
                glMat.texture[i] = albedo.at(i);
            }
        }
        glMaterials.push_back(glMat);
    }

    for (auto& bone : bones)
    {
        gl::Bone glBone{};
        memset(glBone.boneName, 0, 256);
        for (int i = 0; i < bone.name.size(); i++)
        {
            glBone.boneName[i] = bone.name.at(i);
        }
        glBone.parent = bone.parentBoneIndex;
        glBone.translate[0] = bone.position.x;
        glBone.translate[1] = bone.position.y;
        glBone.translate[2] = bone.position.z;
        glBones.push_back(glBone);
    }
    auto mesh = Glib::GLObject{ glVertices, glIndices, glSubsets, glMaterials, glBones };
    return mesh.WriteFile(std::string{ path } + ".globj");
}
