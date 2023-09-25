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
         * @brief �Đ�
         */
        void Play();

        /**
         * @brief �ꎞ��~
         */
        void Pause();

        /**
         * @brief ��~
         */
        void Stop();

        /**
         * @brief �X�^�[�g���ɍĐ����邩
         */
        bool PlayOnStart() const;

        /**
         * @brief �X�^�[�g���ɍĐ����邩
         */
        void PlayOnStart(bool enable);

        /**
         * @brief 3D�T�E���h��
         */
        bool Is3DSound() const;

        /**
         * @brief 3D�T�E���h��
         */
        void Is3DSound(bool enable);

        /**
         * @brief ���[�v���邩
         */
        bool Loop() const;

        /**
         * @brief ���[�v���邩
         */
        void Loop(bool enable);

        /**
         * @brief ����
         */
        float Volume() const;

        /**
         * @brief ���ʂ̐ݒ�
         * @param volume
         */
        void Volume(float volume);

        /**
         * @brief ID�̎擾
         */
        unsigned int AudioID() const;

        /**
         * @brief ID��ݒ�
         */
        void AudioID(unsigned int id);

        /**
         * @brief �O���[�v��ݒ�
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
