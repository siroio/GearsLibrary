#pragma once
#include <string>
#include <vector>

namespace Glib
{
    class GLAnimation
    {
#pragma pack(push, 1)
        struct Header
        {
            char signature[10];
            float version;
            char endian[2];
        };

        struct BoneInfo
        {
            BoneInfo() = default;
            BoneInfo(int count) : boneCount{ count }
            {};
            int boneCount;
        };

        struct MotionInfo
        {
            std::string boneName;
            int keyFrameCount{};
        };

        struct KeyFrame
        {
            int keyFrame{};
            float scale[3];
            float rotation[4];
            float translation[3];
        };

        struct MotionData
        {
            MotionInfo info;
            std::vector<KeyFrame> frames;
        };
#pragma pack(pop)

    public:
        GLAnimation() = default;

        /**
         * @brief �I�u�W�F�N�g���쐬
         * @param motion ���[�V�����f�[�^
         */
        GLAnimation(const BoneInfo& boneInfo, const std::vector<MotionData>& motion);

        /**
         * @brief �t�@�C���̓ǂݍ���
         * @param path
         * @return ���� : true
         * @return ���s : false
         */
        bool ReadFile(std::string_view path);

        /**
         * @brief �t�@�C���֏�������
         * @param path
         * @return ���� : true
         * @return ���s : false
         */
        bool WriteFile(const std::string& path);

    private:
        // == �e��ǂݍ��ݗp�֐� == //

        void ReadHeader(std::ifstream& file);
        void ReadBoneInfo(std::ifstream& file);
        void ReadMotionData(std::ifstream& file);
        void ReadMotionInfo(std::ifstream& file, MotionInfo& info);
        void ReadKeyFrame(std::ifstream& file, KeyFrame& keyFrame);

        // == �e�폑�����ݗp�֐� == //

        void WriteHeader(std::ofstream& file);
        void WriteBoneInfo(std::ofstream& file);
        void WriteMotionData(std::ofstream& file);
        void WriteMotionInfo(std::ofstream& file, const MotionInfo& info);
        void WriteKeyFrame(std::ofstream& file, const KeyFrame& keyFrame);

    private:
        std::string             signature_{ "" };
        float                   version_{ 1.0f };
        std::string             endianInfo_{ "" };
        BoneInfo                boneInfo_{ -1 };
        std::vector<MotionData> motionData_;
    };
}
