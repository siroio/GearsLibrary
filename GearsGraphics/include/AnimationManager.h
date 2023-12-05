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
         * @brief �A�j���[�V�����̓ǂݍ���
         * @param id �A�j���[�V����ID
         * @param filePath �t�@�C���p�X
         * @return ���� : true
         * @return ���s : false
         */
        static bool Load(unsigned int id, std::string_view filePath);

        /**
         * @brief �A�j���[�V�������擾
         * @param id �A�j���[�V����ID
         */
        WeakPtr<AnimationClip> Animation(unsigned int id);
    };
}
