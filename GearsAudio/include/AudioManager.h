#pragma once
#include  <Internal/ISystem.h>
#include  <Singleton.h>

namespace Glib
{
    class AudioManager final :
        public Internal::Interface::ISystem,
        public SingletonPtr<AudioManager>
    {
        AudioManager() = default;
        friend WeakPtr<AudioManager> SingletonPtr<AudioManager>::Instance();

    public:
        /**
         * @brief グループの追加
         * @param groupId グループID
         */
        static void AddSoundGroup(unsigned int groupId);

        /**
         * @brief グループの音量をセット
         * @param groupId グループID
         * @param volume 音量
         */
        static void SetSoundGroupVolume(unsigned int groupId, float volume);

        /**
         * @brief 音声を読み込む
         */
        static bool LoadVoice(unsigned int groupId, std::string_view path);
    };
}
