#pragma once
#include <Component.h>
#include <BitFlag.h>
#include <AudioClip.h>
#include <x3daudio.h>

namespace Glib
{
    class Transform;

    class AudioSource : public Component
    {
        enum class AudioStatus
        {
            PlayOnStart,
            Is3DAudio,
            Pause
        };
    public:
        AudioSource();
        ~AudioSource() override;

    public:
        void Start();
        void LateUpdate();

        /**
         * @brief 再生
         */
        void Play();

        /**
         * @brief 一時停止
         */
        void Pause();

        /**
         * @brief 停止
         */
        void Stop();

        /**
         * @brief スタート時に再生するか
         */
        bool PlayOnStart() const;

        /**
         * @brief スタート時に再生するか
         */
        void PlayOnStart(bool enable);

        /**
         * @brief 3Dサウンドか
         */
        bool Is3DSound() const;

        /**
         * @brief 3Dサウンドか
         */
        void Is3DSound(bool enable);

        /**
         * @brief ループするか
         */
        bool Loop() const;

        /**
         * @brief ループするか
         */
        void Loop(bool enable);

        /**
         * @brief 音量
         */
        float Volume() const;

        /**
         * @brief 音量の設定
         * @param volume
         */
        void Volume(float volume);

        /**
         * @brief IDの取得
         */
        unsigned int AudioID() const;

        /**
         * @brief IDを設定
         */
        void AudioID(unsigned int id);

        /**
         * @brief グループを設定
         */
        void SetGroup(int group);

    private:
        void UpdateVoice();

    private:
        BitFlag<AudioStatus>  audioStatus_{};
        WeakPtr<AudioClip> audioClip_{ nullptr };
        IXAudio2SourceVoice* sourceVoice_{ nullptr };
        X3DAUDIO_EMITTER emitter_{};
        std::unique_ptr<float[]> emitterAzimuths_{ nullptr };
        unsigned int audioId_{ UINT_MAX };
        int groupId_{ -1 };
    };
}

