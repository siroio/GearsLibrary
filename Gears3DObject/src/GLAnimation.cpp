#include <GLAnimation.h>
#include <filesystem>
#include <fstream>
#include <Utility/StringUtility.h>

namespace
{
    /**
     * @brief 拡張子
     */
    constexpr char GL_OBJECT_EXTENSION[]{ "glanim" };

    /**
     * @brief シグネチャ
     */
    constexpr char GL_ANIMATION_SIGNATURE[]{ "GLANIMFILE" };

    /**
     * @brief ファイルバージョン
     */
    constexpr auto GL_ANIMATION_VERSION{ 1.0f };
}

Glib::GLAnimation::GLAnimation(const BoneInfo& boneInfo, const MotionInfo& motionInfo, const std::vector<KeyFrame>& keyframes)
    : boneInfo_{ boneInfo }, motionInfo_{ motionInfo }, keyFrames_{ keyframes }
{}

bool Glib::GLAnimation::ReadFile(std::string_view path)
{
    if (!CheckExt(path, GL_OBJECT_EXTENSION)) return false;
    std::ofstream file{ path.data(), std::ios::binary };
    return false;
}

bool Glib::GLAnimation::WriteFile(const std::string& path)
{
    return false;
}

void Glib::GLAnimation::ReadHeader(std::ifstream& stream)
{}

void Glib::GLAnimation::ReadBoneInfo(std::ifstream& stream)
{}

void Glib::GLAnimation::ReadMotionInfo(std::ifstream& stream)
{}

void Glib::GLAnimation::ReadKeyFrame(std::ifstream& stream)
{}

void Glib::GLAnimation::WriteHeader(std::ofstream& stream)
{}

void Glib::GLAnimation::WriteBoneInfo(std::ofstream& stream)
{}

void Glib::GLAnimation::WriteMotionInfo(std::ofstream& stream)
{}

void Glib::GLAnimation::WriteKeyFrame(std::ofstream& stream)
{}
