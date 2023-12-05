#pragma once
#include <Singleton.h>
#include <AnimationClip.h>

namespace Glib
{
    class AnimationManager : public Singleton<AnimationManager>
    {
        AnimationManager() = default;
        friend AnimationManager& Singleton<AnimationManager>::Instance();

    public:
        /**
         * @brief アニメーションの読み込み
         * @param id アニメーションID
         * @param filePath ファイルパス
         * @return 成功 : true
         * @return 失敗 : false
         */
        static bool Load(unsigned int id, std::string_view filePath);

        /**
         * @brief アニメーションを取得
         * @param id アニメーションID
         */
        WeakPtr<AnimationClip> Animation(unsigned int id);
    };
}
