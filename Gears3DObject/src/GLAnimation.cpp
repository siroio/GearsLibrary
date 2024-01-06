﻿#include <GLAnimation.h>
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

Glib::GLAnimation::GLAnimation(const std::vector<MotionData>& motion)
    : motionData_{ motion }
{
    boneInfo_ = static_cast<int>(motion.size());
}

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

bool Glib::GLAnimation::WriteFile(std::string_view path)
{
    std::ofstream file{ path.data(), std::ios::binary | std::ios::out | std::ios::trunc };

    try
    {
        if (!file.is_open())
        {
            throw std::runtime_error{ "failed to open the file." };
        }

        WriteHeader(file);
        WriteBoneInfo(file);
        WriteMotionData(file);

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
    std::memcpy(&endianInfo_[0], header.endian, 2);
}

void Glib::GLAnimation::ReadBoneInfo(std::ifstream& file)
{
    ReadForBinary(file, &boneInfo_, sizeof(int));
}

void Glib::GLAnimation::ReadMotionData(std::ifstream& file)
{
    if (boneInfo_ < 0)
    {
        throw std::runtime_error{ "invalid bone count." };
    }

    motionData_.resize(boneInfo_);
    for (auto& data : motionData_)
    {
        ReadText(file, data.boneName);
        ReadForBinary(file, &data.frameNo, sizeof(MotionData::frameNo));
        ReadForBinary(file, &data.translation, sizeof(MotionData::translation));
        ReadForBinary(file, &data.rotation, sizeof(MotionData::rotation));
        ReadForBinary(file, &data.scale, sizeof(MotionData::scale));
    }
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
    WriteToBinary(file, &boneInfo_, sizeof(int));
}

void Glib::GLAnimation::WriteMotionData(std::ofstream& file)
{
    if (boneInfo_ < 0)
    {
        throw std::runtime_error{ "invalid bone count." };
    }

    for (auto& data : motionData_)
    {
        // モーションの書き出し
        if (!data.boneName.ends_with('\0')) data.boneName += '\0';
        WriteText(file, data.boneName);
        WriteToBinary(file, &data.frameNo, sizeof(MotionData::frameNo));
        WriteToBinary(file, &data.translation, sizeof(MotionData::translation));
        WriteToBinary(file, &data.rotation, sizeof(MotionData::rotation));
        WriteToBinary(file, &data.scale, sizeof(MotionData::scale));
    }
}

const std::vector<Glib::GLAnimation::MotionData>& Glib::GLAnimation::Motions() const
{
    return motionData_;
}

const int Glib::GLAnimation::FrameLength() const
{
    return boneInfo_;
}
