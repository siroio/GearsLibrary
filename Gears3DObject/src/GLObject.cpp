#include <GLObject.h>
#include <GLObjectConstant.h>
#include <ByteUtility.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

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
    // バイナリモードで開く
    std::ifstream file{ path.data(), std::ios::binary };

    try
    {
        std::filesystem::path check{ path };
        auto c = check.extension().generic_string();
        if (!check.extension().generic_string().ends_with(GL_OBJECT_EXTENSION))
        {
            throw std::runtime_error{ "mismatch file extension." };
        }
        if (!std::filesystem::exists(path))
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
        puts(e.what());
        return false;
    }
}

bool Glib::GLObject::WriteFile(const std::string& path)
{
    std::ofstream file{ path, std::ios::binary | std::ios::out | std::ios::trunc };

    try
    {
        if (path.length() >= FILENAME_MAX)
        {
            throw std::runtime_error{ "file name is too long." };
        }
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
        std::cout << e.what() << std::endl;
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

void Glib::GLObject::ReadHeader(std::ifstream& stream)
{
    // ヘッダの読み込み
    Header header{};
    stream.read(reinterpret_cast<char*>(&header), sizeof(Header));

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
    std::memcpy(endianInfo_, header.endian, 2);
}

void Glib::GLObject::ReadVertex(std::ifstream& stream)
{
    // 頂点数読み込み
    int vertexCount{ 0 };
    stream.read(reinterpret_cast<char*>(&vertexCount), sizeof(int));

    if (vertexCount <= 0)
    {
        throw std::runtime_error{ "invalid vertex length." };
    }


    // 頂点読み込み
    std::vector<Vertex> vertices(vertexCount);
    stream.read(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex) * vertexCount);

    if (vertices.size() <= 0)
    {
        throw std::runtime_error{ "invalid vertex." };
    }

    vertices_ = std::move(vertices);
}

void Glib::GLObject::ReadIndex(std::ifstream& stream)
{
    // インデックス数読み込み
    int indexCount{ 0 };
    stream.read(reinterpret_cast<char*>(&indexCount), sizeof(int));

    if (indexCount <= 0)
    {
        throw std::runtime_error{ "invalid index length." };
    }

    // インデックス読み込み
    std::vector<unsigned int> indices(indexCount);
    stream.read(reinterpret_cast<char*>(indices.data()), sizeof(unsigned int) * indexCount);

    if (indices.size() <= 0)
    {
        throw std::runtime_error{ "invalid index." };
    }

    indices_ = std::move(indices);
}

void Glib::GLObject::ReadSubset(std::ifstream& stream)
{
    // サブセット数の読み込み
    int subsetCount{ 0 };
    stream.read(reinterpret_cast<char*>(&subsetCount), sizeof(int));

    if (subsetCount <= 0)
    {
        throw std::runtime_error{ "invalid subset length." };
    }

    // サブセットの読み込み
    std::vector<Subset> subsets(subsetCount);
    stream.read(reinterpret_cast<char*>(subsets.data()), sizeof(Subset) * subsetCount);

    if (subsets.size() <= 0)
    {
        throw std::runtime_error{ "invalid material." };
    }

    subsets_ = std::move(subsets);
}

void Glib::GLObject::ReadMaterial(std::ifstream& stream)
{
    // マテリアル数の読み込み
    int materialCount{ 0 };
    stream.read(reinterpret_cast<char*>(&materialCount), sizeof(int));

    if (materialCount <= 0)
    {
        throw std::runtime_error{ "invalid material length." };
    }

    // マテリアルの読み込み
    std::vector<Material> materials(materialCount);
    stream.read(reinterpret_cast<char*>(materials.data()), sizeof(Material) * materialCount);

    if (materials.size() <= 0)
    {
        throw std::runtime_error{ "invalid material." };
    }

    materials_ = std::move(materials);
}

void Glib::GLObject::ReadBone(std::ifstream& stream)
{
    // ボーン数の読み込み
    int boneCount{ 0 };
    stream.read(reinterpret_cast<char*>(&boneCount), sizeof(int));

    if (boneCount <= 0)
    {
        throw std::runtime_error{ "invalid bone length." };
    }

    // ボーンの読み込み
    std::vector<Bone> bones(boneCount);
    stream.read(reinterpret_cast<char*>(bones.data()), sizeof(Bone) * boneCount);

    bones_ = std::move(bones);
}

void Glib::GLObject::WriteHeader(std::ofstream& stream)
{
    Header header{};
    if (signature_.empty())
    {
        // ヘッダーがない場合新規に作成
        std::memcpy(header.signature, GL_OBJECT_SIGNATURE, sizeof(GL_OBJECT_SIGNATURE) - 1);
        header.version = GL_OBJECT_VERSION;
        std::memcpy(header.endian, Glib::CheckEndian().c_str(), sizeof(header.endian));
    }
    else
    {
        // ヘッダの読み込み
        std::memcpy(header.signature, signature_.c_str(), sizeof(GL_OBJECT_SIGNATURE) - 1);
        header.version = version_;
        std::memcpy(header.endian, endianInfo_, sizeof(header.endian));
    }
    stream.write(reinterpret_cast<char*>(&header), sizeof(Header));
}

void Glib::GLObject::WriteVertex(std::ofstream& stream)
{
    int vertexCount = static_cast<int>(vertices_.size());

    // 頂点数を書き込み
    stream.write(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));

    // 頂点を書き込み
    stream.write(reinterpret_cast<char*>(vertices_.data()), sizeof(Vertex) * vertexCount);
}

void Glib::GLObject::WriteIndex(std::ofstream& stream)
{
    int indexCount = static_cast<int>(indices_.size());

    // インデックス数の書き込み
    stream.write(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));

    // インデックスの書き込み
    stream.write(reinterpret_cast<char*>(indices_.data()), sizeof(unsigned int) * indexCount);
}

void Glib::GLObject::WriteSubset(std::ofstream& stream)
{
    int subsetCount = static_cast<int>(subsets_.size());

    // サブセット数の書き込み
    stream.write(reinterpret_cast<char*>(&subsetCount), sizeof(subsetCount));

    // サブセットの書き込み
    stream.write(reinterpret_cast<char*>(subsets_.data()), sizeof(Subset) * subsetCount);
}

void Glib::GLObject::WriteMaterial(std::ofstream& stream)
{
    int materialCount = static_cast<int>(subsets_.size());

    // マテリアル数の書き込み
    stream.write(reinterpret_cast<char*>(&materialCount), sizeof(materialCount));

    // マテリアルの書き込み
    stream.write(reinterpret_cast<char*>(materials_.data()), sizeof(Material) * materialCount);
}

void Glib::GLObject::WriteBone(std::ofstream& stream)
{
    int boneCount = static_cast<int>(bones_.size());

    // ボーン数の書き込み
    stream.write(reinterpret_cast<char*>(&boneCount), sizeof(boneCount));

    // ボーンの書き込み
    stream.write(reinterpret_cast<char*>(bones_.data()), sizeof(Bone) * boneCount);
}
