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
     * @brief �X�L�j���O�`��p���R���|�[�l���g
     */
    class SkinnedRenderer
    {
    protected:
        SkinnedRenderer() = default;
        virtual ~SkinnedRenderer() = default;

    public:
        /**
         * @brief �{�[���̎擾
         */
        virtual const std::vector<Bone>& Bones() const;

        /**
         * @brief �{�[���̃g�����X�t�H�[�����擾
         */
        virtual const std::vector<WeakPtr<Transform>>& BoneTransforms() const;

        /**
         * @brief �{�[���̍s����擾
         */
        virtual std::array<Matrix4x4, 128>& BoneMatrix();
    };
}
