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
            int boneCount;
        };

        struct MotionInfo
        {
            std::string boneName;
            int keyFrameCount;
        };

        struct KeyFrame
        {
            int keyFrame;
            float scale[3];
            float rotation[4];
            float translation[3];
        };
#pragma pack(pop)

    public:
        GLAnimation() = default;

        /**
         * @brief オブジェクトを作成
         * @param info モーションの情報
         * @param keyframes キーフレーム
         */
        GLAnimation(const BoneInfo& boneInfo, const MotionInfo& motionInfo,
                    const std::vector<KeyFrame>& keyframes);

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
        bool WriteFile(const std::string& path);

    private:
        // == 各種読み込み用関数 == //

        void ReadHeader(std::ifstream& stream);
        void ReadBoneInfo(std::ifstream& stream);
        void ReadMotionInfo(std::ifstream& stream);
        void ReadKeyFrame(std::ifstream& stream);

        // == 各種書き込み用関数 == //

        void WriteHeader(std::ofstream& stream);
        void WriteBoneInfo(std::ofstream& stream);
        void WriteMotionInfo(std::ofstream& stream);
        void WriteKeyFrame(std::ofstream& stream);

    private:
        std::string signature_{ "" };
        float version_{ 1.0f };
        char endianInfo_[2]{ "" };
        BoneInfo boneInfo_{};
        MotionInfo motionInfo_{};
        std::vector<KeyFrame> keyFrames_;
    };
}
