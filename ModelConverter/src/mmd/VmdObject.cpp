#include <mmd/VmdObject.h>
#include <utility/FileUtility.h>
#include <GLAnimation.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

namespace
{
    constexpr char VMD_FILE_EXTENSION[]{ "vmd" };
}

bool VmdMotion::LoadFile(std::string_view path)
{
    // 拡張子が間違っていたら失敗
    if (!FileUtility::GetExtension(path).ends_with(VMD_FILE_EXTENSION)) return false;
    std::ifstream vmdFile{ path.data(), std::ios::binary };
    if (vmdFile.fail()) return false;

    // ヘッダをスキップ(50バイト)
    vmdFile.seekg(50, std::ios::beg);
    ReadKeyFrames(vmdFile);

    return true;
}

bool VmdMotion::WriteFile(std::string_view path)
{
    using gl = Glib::GLAnimation;
    std::vector<gl::MotionData> motionDatas;

    for (auto& keyFrame : keyFrames_)
    {
        gl::MotionData motionData{};
        // モーションの情報
        std::string boneName = std::string{ keyFrame.boneName };
        if (!boneName.empty() && boneName.back() != '\0')
        {
            boneName.push_back('\0');
        }
        motionData.boneName = std::move(boneName);
        motionData.frameNo = keyFrame.frameNo;

        // キーフレームの変換
        motionData.frameNo = keyFrame.frameNo;
        std::copy(std::begin(keyFrame.translate.elem), std::end(keyFrame.translate.elem), motionData.translation);
        std::copy(std::begin(keyFrame.rotation.elem), std::end(keyFrame.rotation.elem), motionData.rotation);
        motionData.scale[0] = 1.0f;
        motionData.scale[1] = 1.0f;
        motionData.scale[2] = 1.0f;

        // キーフレームの追加
        motionDatas.push_back(motionData);
    }

    Glib::GLAnimation anim{ motionDatas };
    return anim.WriteFile(std::string{ path } + ".glanim");
}

void VmdMotion::ReadKeyFrames(std::ifstream& file)
{
    int keyFrameCount;
    FileUtility::ReadForBinary(file, &keyFrameCount, sizeof(unsigned int));
    keyFrames_.resize(keyFrameCount);

    for (auto& keyframe : keyFrames_)
    {
        keyframe.boneName = std::string(15, '\0');
        FileUtility::ReadText(file, keyframe.boneName, sizeof(char) * 15);
        FileUtility::ReadForBinary(file, &keyframe.frameNo, sizeof(KeyFrame::frameNo));
        FileUtility::ReadForBinary(file, keyframe.translate.elem, sizeof(float) * 3);
        FileUtility::ReadForBinary(file, keyframe.rotation.elem, sizeof(float) * 4);
        FileUtility::ReadForBinary(file, keyframe.bezier, sizeof(KeyFrame::bezier));
    }
    keyFrames_.shrink_to_fit();
}
