#include <GLObject.h>
#include <Utility/ByteUtility.h>
#include <Utility/StringUtility.h>
#include <Utility/IOUtility.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

namespace
{
    /**
     * @brief 拡張子
     */
    constexpr char GL_OBJECT_EXTENSION[]{ "globj" };

    /**
     * @brief シグネチャ
     */
    constexpr char GL_OBJECT_SIGNATURE[]{ "GLOBJFILE" };

    /**
     * @brief ファイルバージョン
     */
    constexpr auto GL_OBJECT_VERSION{ 1.0f };
}

Glib::GLObject::GLObject
(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<Subset>& subsets,
    const std::vector<Material>& materials,
    const std::vector<Bone>& bones
)
{
    vertices_ = vertices;
    indices_ = indices;
    subsets_ = subsets;
    materials_ = materials;
    bones_ = bones;
}

bool Glib::GLObject::ReadFile(std::string_view path)
{
    try
    {
        // バイナリモードで開く
        std::ifstream file{ path.data(), std::ios::binary };

        if (!CheckExt(path, GL_OBJECT_EXTENSION))
        {
            throw std::runtime_error{ "mismatch file extension." };
        }

        // 絶対パスに変換
        auto filePath = GetAbsPath(path);
        if (!std::filesystem::exists(filePath))
        {
            throw std::runtime_error{ "file not found." };
        }
        if (!file.is_open())
        {
            throw std::runtime_error{ "failed to open the file." };
        }

        ReadHeader(file);
        ReadVertex(file);
        ReadIndex(file);
        ReadSubset(file);
        ReadMaterial(file);
        ReadBone(file);

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool Glib::GLObject::WriteFile(std::string_view path)
{
    try
    {
        std::ofstream file{ path.data(), std::ios::binary | std::ios::out | std::ios::trunc };
        if (!file.is_open())
        {
            throw std::runtime_error{ "failed to open the file." };
        }

        WriteHeader(file);
        WriteVertex(file);
        WriteIndex(file);
        WriteSubset(file);
        WriteMaterial(file);
        WriteBone(file);

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::vector<Glib::GLObject::Vertex>& Glib::GLObject::Vertices()
{
    return vertices_;
}

std::vector<unsigned int>& Glib::GLObject::Indices()
{
    return indices_;
}

std::vector<Glib::GLObject::Subset>& Glib::GLObject::Subsets()
{
    return subsets_;
}

std::vector<Glib::GLObject::Material>& Glib::GLObject::Materials()
{
    return materials_;
}

std::vector<Glib::GLObject::Bone>& Glib::GLObject::Bones()
{
    return bones_;
}

void Glib::GLObject::ReadHeader(std::ifstream& file)
{
    // ヘッダの読み込み
    Header header{};
    ReadForBinary(file, &header, sizeof(Header));

    // シグネチャが正しいか検証
    if (strncmp(header.signature, GL_OBJECT_SIGNATURE, 9) != 0)
    {
        throw std::runtime_error{ "invalid file signature." };
    }

    if (GL_OBJECT_VERSION != header.version)
    {
        throw std::runtime_error{ "version is incompatible." };
    }

    version_ = header.version;
    signature_ = header.signature;
    std::memset(header.endian, 0, 2);
    endianInfo_ = header.endian;
}

void Glib::GLObject::ReadVertex(std::ifstream& file)
{
    // 頂点数読み込み
    int vertexCount{ 0 };
    ReadForBinary(file, &vertexCount, sizeof(int));

    if (vertexCount <= 0)
    {
        throw std::runtime_error{ "invalid vertex length." };
    }


    // 頂点読み込み
    vertices_.resize(vertexCount);
    ReadForBinary(file, vertices_.data(), sizeof(Vertex) * vertexCount);

    if (vertices_.empty())
    {
        throw std::runtime_error{ "invalid vertex." };
    }
}

void Glib::GLObject::ReadIndex(std::ifstream& file)
{
    // インデックス数読み込み
    int indexCount{ 0 };
    ReadForBinary(file, &indexCount, sizeof(int));

    if (indexCount <= 0)
    {
        throw std::runtime_error{ "invalid index length." };
    }

    // インデックス読み込み
    indices_.resize(indexCount);
    ReadForBinary(file, indices_.data(), sizeof(unsigned int) * indexCount);

    if (indices_.empty())
    {
        throw std::runtime_error{ "invalid index." };
    }
}

void Glib::GLObject::ReadSubset(std::ifstream& file)
{
    // サブセット数の読み込み
    int subsetCount{ 0 };
    ReadForBinary(file, &subsetCount, sizeof(int));

    if (subsetCount <= 0)
    {
        throw std::runtime_error{ "invalid subset length." };
    }

    // サブセットの読み込み
    subsets_.resize(subsetCount);
    ReadForBinary(file, subsets_.data(), sizeof(Subset) * subsetCount);

    if (subsets_.empty())
    {
        throw std::runtime_error{ "invalid material." };
    }
}

void Glib::GLObject::ReadMaterial(std::ifstream& file)
{
    // マテリアル数の読み込み
    int materialCount{ 0 };
    ReadForBinary(file, &materialCount, sizeof(int));

    if (materialCount <= 0)
    {
        throw std::runtime_error{ "invalid material length." };
    }

    // マテリアルの読み込み
    materials_.resize(materialCount);
    for (auto& material : materials_)
    {
        ReadForBinary(file, &material.ambient, sizeof(Material::ambient));
        ReadForBinary(file, &material.diffuse, sizeof(Material::diffuse));
        ReadForBinary(file, &material.specular, sizeof(Material::specular));
        ReadForBinary(file, &material.shininess, sizeof(Material::shininess));
        ReadText(file, material.texture);
        ReadText(file, material.normal);
    }

    if (materials_.empty())
    {
        throw std::runtime_error{ "invalid material." };
    }
}

void Glib::GLObject::ReadBone(std::ifstream& file)
{
    // ボーン数の読み込み
    int boneCount{ 0 };
    ReadForBinary(file, &boneCount, sizeof(int));

    if (boneCount < 0)
    {
        throw std::runtime_error{ "invalid bone length." };
    }

    // ボーンの読み込み
    bones_.resize(boneCount);
    for (auto& bone : bones_)
    {
        ReadText(file, bone.boneName);
        ReadForBinary(file, &bone.translate, sizeof(Bone::translate));
        ReadForBinary(file, &bone.parent, sizeof(Bone::parent));
    }
}

void Glib::GLObject::WriteHeader(std::ofstream& file)
{
    Header header{};
    if (signature_.empty())
    {
        // ヘッダーがない場合新規に作成
        // -1 はnull文字を含ませないため
        std::memcpy(header.signature, GL_OBJECT_SIGNATURE, sizeof(GL_OBJECT_SIGNATURE) - 1);
        header.version = GL_OBJECT_VERSION;
        std::memcpy(header.endian, Glib::GetEndian().c_str(), sizeof(header.endian));
    }
    else
    {
        // ヘッダの読み込み
        std::memcpy(header.signature, signature_.c_str(), sizeof(GL_OBJECT_SIGNATURE) - 1);
        header.version = version_;
        std::memcpy(header.endian, &endianInfo_[0], sizeof(header.endian));
    }
    WriteToBinary(file, &header, sizeof(Header));
}

void Glib::GLObject::WriteVertex(std::ofstream& file)
{
    int vertexCount = static_cast<int>(vertices_.size());

    // 頂点数を書き込み
    WriteToBinary(file, &vertexCount, sizeof(int));

    // 頂点を書き込み
    WriteToBinary(file, vertices_.data(), sizeof(Vertex) * vertexCount);
}

void Glib::GLObject::WriteIndex(std::ofstream& file)
{
    int indexCount = static_cast<int>(indices_.size());

    // インデックス数の書き込み
    WriteToBinary(file, &indexCount, sizeof(int));

    // インデックスの書き込み
    WriteToBinary(file, indices_.data(), sizeof(unsigned int) * indexCount);
}

void Glib::GLObject::WriteSubset(std::ofstream& file)
{
    int subsetCount = static_cast<int>(subsets_.size());

    // サブセット数の書き込み
    WriteToBinary(file, &subsetCount, sizeof(int));

    // サブセットの書き込み
    WriteToBinary(file, subsets_.data(), sizeof(Subset) * subsetCount);
}

void Glib::GLObject::WriteMaterial(std::ofstream& file)
{
    int materialCount = static_cast<int>(subsets_.size());

    // マテリアル数の書き込み
    WriteToBinary(file, &materialCount, sizeof(int));

    // マテリアルの書き込み
    for (auto& material : materials_)
    {
        WriteToBinary(file, &material.ambient, sizeof(Material::ambient));
        WriteToBinary(file, &material.diffuse, sizeof(Material::diffuse));
        WriteToBinary(file, &material.specular, sizeof(Material::specular));
        WriteToBinary(file, &material.shininess, sizeof(Material::shininess));
        WriteText(file, material.texture);
        WriteText(file, material.normal);
    }
}

void Glib::GLObject::WriteBone(std::ofstream& file)
{
    int boneCount = static_cast<int>(bones_.size());

    // ボーン数の書き込み
    WriteToBinary(file, &boneCount, sizeof(int));

    // ボーンの書き込み
    for (auto& bone : bones_)
    {
        if (!bone.boneName.ends_with('\0')) bone.boneName += '\0';
        WriteText(file, bone.boneName);
        WriteToBinary(file, &bone.translate, sizeof(Bone::translate));
        WriteToBinary(file, &bone.parent, sizeof(Bone::parent));
    }
}
