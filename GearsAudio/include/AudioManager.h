#pragma once
#include  <Internal/ISystem.h>
#include  <Singleton.h>

namespace Glib
{
    class AudioManager final :
        public Internal::Interface::ISystem,
        public Glib::SingletonPtr<AudioManager>
    {
        AudioManager() = default;
        friend WeakPtr<AudioManager> Glib::SingletonPtr<AudioManager>::Instance();

    public:
        /**
         * @brief �O���[�v�̒ǉ�
         * @param groupId �O���[�vID
         */
        void AddSoundGroup(unsigned int groupId);

        /**
         * @brief �O���[�v�̉��ʂ��Z�b�g
         * @param groupId �O���[�vID
         * @param volume ����
         */
        void SetSoundGroupVolume(unsigned int groupId, float volume);

        /**
         * @brief ������ǂݍ���
         */
        void LoadVoice(unsigned int groupId, std::string_view path);
    };
}
