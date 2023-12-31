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
    // �g���q���Ԉ���Ă����玸�s
    if (!FileUtility::GetExtension(path).ends_with(VMD_FILE_EXTENSION)) return false;
    std::ifstream vmdFile{ path.data(), std::ios::binary };
    if (vmdFile.fail()) return false;

    // �w�b�_���X�L�b�v(50�o�C�g)
    vmdFile.seekg(50, std::ios::beg);
    ReadKeyFrames(vmdFile);

    return true;
}

bool VmdMotion::WriteFile(std::string_view path)
{
    using gl = Glib::GLAnimation;
    gl::BoneInfo info{};
    std::vector<gl::MotionData> motionDatas;

    info.boneCount = keyFrameCount_;
    for (auto& keyFrame : keyFrames_)
    {
        gl::MotionData motionData{};
        // ���[�V�����̏��
        std::string boneName = std::string{ keyFrame.boneName };
        if (!boneName.empty() && boneName.back() != '\0')
        {
            boneName.push_back('\0');
        }
        motionData.info.boneName = std::move(boneName);
        motionData.info.keyFrameCount = keyFrame.frameNo;

        // �L�[�t���[���̕ϊ�
        gl::KeyFrame destKeyFrame{};
        destKeyFrame.frameNo = keyFrame.frameNo;
        std::copy(std::begin(keyFrame.translate.elem), std::end(keyFrame.translate.elem), destKeyFrame.translation);
        std::copy(std::begin(keyFrame.rotation.elem), std::end(keyFrame.rotation.elem), destKeyFrame.rotation);
        destKeyFrame.scale[0] = 1.0f;
        destKeyFrame.scale[1] = 1.0f;
        destKeyFrame.scale[2] = 1.0f;
        motionData.frame = std::move(destKeyFrame);

        // �L�[�t���[���̒ǉ�
        motionDatas.push_back(motionData);
    }

    Glib::GLAnimation animation{ info, motionDatas };
    animation.WriteFile(std::string{ path } + ".glanim");
    return true;
}

void VmdMotion::ReadKeyFrames(std::ifstream& file)
{
    FileUtility::ReadForBinary(file, &keyFrameCount_, sizeof(unsigned int));
    for (unsigned int i = 0; i < keyFrameCount_; i++)
    {
        KeyFrame frame{};
        char buffer[15]{};
        FileUtility::ReadForBinary(file, &buffer, sizeof(buffer));
        frame.boneName = std::string{ buffer };
        FileUtility::ReadForBinary(file, &frame.frameNo, sizeof(KeyFrame::frameNo));
        FileUtility::ReadForBinary(file, &frame.translate, sizeof(KeyFrame::translate));
        FileUtility::ReadForBinary(file, &frame.rotation, sizeof(KeyFrame::rotation));
        FileUtility::ReadForBinary(file, &frame.bezier, sizeof(KeyFrame::bezier));
        keyFrames_.push_back(frame);
    }
    keyFrames_.shrink_to_fit();
}
