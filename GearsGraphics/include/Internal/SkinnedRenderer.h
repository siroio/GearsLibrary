#pragma once
#include <array>
#include <vector>

struct Matrix4x4;

namespace Glib
{
    template<class T>
    class WeakPtr;
    struct Bone;
    class Transform;
}

namespace Glib::Internal
{
    /**
     * @brief スキニング描画用基底コンポーネント
     */
    class SkinnedRenderer
    {
    protected:
        SkinnedRenderer() = default;
        virtual ~SkinnedRenderer() = default;

    public:
        /**
         * @brief ボーンの取得
         */
        virtual const std::vector<Bone>& Bones() const = 0;

        /**
         * @brief ボーンのトランスフォームを取得
         */
        virtual const std::vector<WeakPtr<Transform>>& BoneTransforms() const = 0;

        /**
         * @brief ボーンの行列を取得
         */
        virtual std::array<Matrix4x4, 512>& BoneMatrix() = 0;
    };
}

