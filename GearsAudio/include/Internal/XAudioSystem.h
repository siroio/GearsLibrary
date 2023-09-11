#pragma once
#include  <Internal/ISystem.h>
#include  <Singleton.h>

struct IXAudio2SourceVoice;
struct IXAudio2SubmixVoice;
struct X3DAUDIO_EMITTER;
struct XAUDIO2_BUFFER;
struct Vector3;

namespace Glib
{
    class AudioClip;
}

namespace Glib::Internal::Audio
{
    class XAudioSystem final :
        public Interface::ISystem,
        public SingletonPtr<XAudioSystem>
    {
        XAudioSystem() = default;
        friend  WeakPtr<XAudioSystem> SingletonPtr<XAudioSystem>::Instance();

    public:
        bool Initialize();
        void Finalize();

        /**
         * @brief ������ǂݍ���
         */
        bool LoadVoice(unsigned int id, std::string_view path);

        /**
         * @brief 3DAudio�̌v�Z
         */
        void Audio3DCalculate(const X3DAUDIO_EMITTER* emitter, IXAudio2SourceVoice* voice, unsigned int groupId);

        /**
         * @brief SourceVoice���쐬
         */
        void CreateSourceVoice(unsigned int id, bool loop, WeakPtr<AudioClip>& clip, IXAudio2SourceVoice** voice);

        /**
         * @brief SubMixVoice���쐬
         */
        void CreateSubMixVoice(unsigned int groupId);

        /**
         * @brief SubMixVoice�̏o�͐���w��
         */
        void SetOutputSubMixVoice(IXAudio2SourceVoice* sourceVoice, unsigned int groupId);

        /**
         * @brief ���X�i�[�̃p�����[�^�[��ݒ�
         * @param position �ʒu
         * @param forward �O
         * @param up ��
         */
        void SetListenerParameter(const Vector3& position, const Vector3& forward, const Vector3& up);

        /**
         * @brief
         * @param groupId �O���[�v�̉��ʂ�ݒ�
         * @param volume ����
         */
        void SetGroupVolume(unsigned int groupId, float volume);
    };
}
