#pragma once
#include <string>
#include <unordered_map>
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix4x4.h>

namespace Glib
{
    /**
     * @brief アニメーション管理クラス
     */
    class AnimationClip
    {
    private:
        struct KeyFrame
        {
            KeyFrame() = default;
            float frameNo{ 0.0f };
            Vector3 position;
            Quaternion rotation;
            Vector3 scale;
            Matrix4x4 Matrix() const;
        };

        using KeyFrames = std::vector<KeyFrame>;

    public:
        /**
         * @brief アニメーションを読み込み
         * @param fileName ファイルパス
         */
        bool Load(std::string_view fileName);

        /**
         * @brief キーフレームの取得
         * @param boneName  ボーン名
         * @param frameNo   フレーム番号
         * @return キーフレーム
         */
        KeyFrame GetKeyFrame(const std::string& boneName, float frameNo) const;

        /**
         * @brief 終了フレームを取得
         */
        float EndFrame() const;

    private:
        /**
         * @brief キーフレームを検索
         */
        std::pair<int, int> SearchKeyFrame(const KeyFrames& keys, float frameNo) const;

    private:
        float endFrame_;
        std::unordered_map<std::string, KeyFrames> keyframes_;
    };
}
