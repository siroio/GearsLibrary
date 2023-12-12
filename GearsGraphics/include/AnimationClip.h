#pragma once
#include <string>
#include <unordered_map>
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix4x4.h>

namespace Glib
{
    /**
     * @brief �A�j���[�V�����Ǘ��N���X
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
         * @brief �A�j���[�V������ǂݍ���
         * @param fileName �t�@�C���p�X
         */
        bool Load(std::string_view fileName);

        /**
         * @brief �L�[�t���[���̎擾
         * @param boneName  �{�[����
         * @param frameNo   �t���[���ԍ�
         * @return �L�[�t���[��
         */
        KeyFrame GetKeyFrame(std::string_view boneName, float frameNo) const;

        /**
         * @brief �I���t���[�����擾
         */
        float EndFrame() const;

    private:
        /**
         * @brief �L�[�t���[��������
         */
        std::pair<int, int> SearchKeyFrame(const KeyFrames& keys, float frameNo) const;

    private:
        std::unordered_map<std::string, KeyFrames> keyframes_;
    };
}
