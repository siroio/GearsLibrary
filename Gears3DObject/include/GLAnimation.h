#pragma once
#include <string>
#include <vector>

namespace Glib
{
    class GLAnimation
    {
    public:
#pragma pack(push, 1)
        struct Header
        {
            char signature[10];
            float version;
            char endian[2];
        };

        struct BoneInfo
        {
            int boneCount;
        };

        struct MotionInfo
        {
            std::string boneName;
            int keyFrameCount{};
        };

        struct KeyFrame
        {
            int frameNo{};
            float scale[3];
            float rotation[4];
            float translation[3];
        };

        struct MotionData
        {
            MotionInfo info;
            KeyFrame frame;
        };
#pragma pack(pop)

    public:
        GLAnimation() = default;

        /**
         * @brief オブジェクトを作成
         * @param motion モーションデータ
         */
        GLAnimation(const BoneInfo& boneInfo, const std::vector<MotionData>& motion);

        /**
         * @brief ファイルの読み込み
         * @param path
         * @return 成功 : true
         * @return 失敗 : false
         */
        bool ReadFile(std::string_view path);

        /**
         * @brief ファイルへ書き込み
         * @param path
         * @return 成功 : true
         * @return 失敗 : false
         */
        bool WriteFile(std::string_view path);

    private:
        // == 各種読み込み用関数 == //

        void ReadHeader(std::ifstream& file);
        void ReadBoneInfo(std::ifstream& file);
        void ReadMotionData(std::ifstream& file);
        void ReadMotionInfo(std::ifstream& file, MotionInfo& info);
        void ReadKeyFrames(std::ifstream& file, KeyFrame& keyFrames);

        // == 各種書き込み用関数 == //

        void WriteHeader(std::ofstream& file);
        void WriteBoneInfo(std::ofstream& file);
        void WriteMotionData(std::ofstream& file);
        void WriteMotionInfo(std::ofstream& file, const MotionInfo& info);
        void WriteKeyFrame(std::ofstream& file, const KeyFrame& keyFrames);

    public:
        const std::vector<MotionData>& Motions() const;
        const BoneInfo& FrameLength() const;

    private:
        std::string             signature_{ "" };
        float                   version_{ 1.0f };
        std::string             endianInfo_{ "" };
        BoneInfo                boneInfo_;
        std::vector<MotionData> motionData_;
    };
}

