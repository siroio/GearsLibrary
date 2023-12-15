#include <GLAnimation.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <Utility/IOUtility.h>
#include <Utility/StringUtility.h>
#include <Utility/ByteUtility.h>

namespace
{
    /**
     * @brief 拡張子
     */
    constexpr char GL_ANIM_EXTENSION[]{ "glanim" };

    /**
     * @brief シグネチャ
     */
    constexpr char GL_ANIM_SIGNATURE[]{ "GLANIMFILE" };

    /**
     * @brief ファイルバージョン
     */
    constexpr auto GL_ANIM_VERSION{ 1.0f };
}

Glib::GLAnimation::GLAnimation(const BoneInfo& boneInfo, const std::vector<MotionData>& motion)
    : boneInfo_{ boneInfo }, motionData_{ motion }
{}

bool Glib::GLAnimation::ReadFile(std::string_view path)
{
    try
    {
        std::ifstream file{ path.data(), std::ios::binary };

        // 正しいファイルかチェック
        if (!CheckExt(path, GL_ANIM_EXTENSION))
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

        // 読み込み
        ReadHeader(file);
        ReadBoneInfo(file);
        ReadMotionData(file);

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool Glib::GLAnimation::WriteFile(const std::string& path)
{
    std::ofstream file{ path, std::ios::binary | std::ios::out | std::ios::trunc };

    try
    {
        if (!file.is_open())
        {
            throw std::runtime_error{ "failed to open the file." };
        }

        WriteHeader(file);


        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void Glib::GLAnimation::ReadHeader(std::ifstream& file)
{
    Header header{};
    ReadForBinary(file, &header, sizeof(header));

    // シグネチャが正しいか検証
    if (strncmp(header.signature, GL_ANIM_SIGNATURE, 10) != 0)
    {
        throw std::runtime_error{ "invalid file signature." };
    }

    if (GL_ANIM_VERSION != header.version)
    {
        throw std::runtime_error{ "version is incompatible." };
    }

    version_ = header.version;
    signature_ = header.signature;
    endianInfo_ = header.endian;
}

void Glib::GLAnimation::ReadBoneInfo(std::ifstream& file)
{
    BoneInfo info{};
    ReadForBinary(file, &info, sizeof(BoneInfo));
    boneInfo_ = info;
}

void Glib::GLAnimation::ReadMotionData(std::ifstream& file)
{
    if (boneInfo_.boneCount < 0)
    {
        throw std::runtime_error{ "invalid bone count." };
    }

    motionData_.resize(boneInfo_.boneCount);
    for (auto& data : motionData_)
    {
        ReadMotionInfo(file, data.info);
        data.frames.resize(data.info.keyFrameCount);
        for (auto& keyFrame : data.frames)
        {
            ReadKeyFrame(file, keyFrame);
        }
    }
}

void Glib::GLAnimation::ReadMotionInfo(std::ifstream& file, MotionInfo& info)
{
    ReadText(file, info.boneName);
    ReadForBinary(file, &info.keyFrameCount, sizeof(info.keyFrameCount));
}

void Glib::GLAnimation::ReadKeyFrame(std::ifstream& file, KeyFrame& keyFrame)
{
    ReadForBinary(file, &keyFrame, sizeof(keyFrame));
}

void Glib::GLAnimation::WriteHeader(std::ofstream& file)
{
    Header header{};
    // signature の -1 はnull文字を含ませないため
    if (signature_.empty())
    {
        // ヘッダーがない場合新規に作成
        std::memcpy(header.signature, GL_ANIM_SIGNATURE, sizeof(GL_ANIM_SIGNATURE) - 1);
        header.version = GL_ANIM_VERSION;
        std::memcpy(header.endian, Glib::GetEndian().c_str(), sizeof(header.endian));
    }
    else
    {
        // ヘッダの読み込み
        std::memcpy(header.signature, signature_.c_str(), sizeof(GL_ANIM_SIGNATURE) - 1);
        header.version = version_;
        std::memcpy(header.endian, &endianInfo_[0], sizeof(header.endian));
    }
    WriteToBinary(file, &header, sizeof(Header));
}

void Glib::GLAnimation::WriteBoneInfo(std::ofstream& file)
{
    WriteToBinary(file, &boneInfo_, sizeof(BoneInfo));
}

void Glib::GLAnimation::WriteMotionData(std::ofstream& file)
{
    if (boneInfo_.boneCount < 0)
    {
        throw std::runtime_error{ "invalid bone count." };
    }

    for (const auto& data : motionData_)
    {
        // モーションの書き出し
        WriteMotionInfo(file, data.info);
        for (const auto& keyFrame : data.frames)
        {
            WriteKeyFrame(file, keyFrame);
        }
    }
}

void Glib::GLAnimation::WriteMotionInfo(std::ofstream& file, const MotionInfo& info)
{
    WriteText(file, info.boneName);
    WriteToBinary(file, &info.keyFrameCount, sizeof(info));
}

void Glib::GLAnimation::WriteKeyFrame(std::ofstream& file, const KeyFrame& keyFrame)
{
    WriteToBinary(file, &keyFrame, sizeof(KeyFrame));
}
