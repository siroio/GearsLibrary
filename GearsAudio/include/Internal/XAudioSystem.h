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
         * @brief 音声を読み込み
         */
        bool LoadVoice(unsigned int id, std::string_view path);

        /**
         * @brief 3DAudioの計算
         */
        void Audio3DCalculate(const X3DAUDIO_EMITTER* emitter, IXAudio2SourceVoice* voice, unsigned int groupId);

        /**
         * @brief SourceVoiceを作成
         */
        void CreateSourceVoice(unsigned int id, bool loop, WeakPtr<AudioClip>& clip, IXAudio2SourceVoice** voice);

        /**
         * @brief SubMixVoiceを作成
         */
        void CreateSubMixVoice(unsigned int groupId);

        /**
         * @brief SubMixVoiceの出力先を指定
         */
        void SetOutputSubMixVoice(IXAudio2SourceVoice* sourceVoice, unsigned int groupId);

        /**
         * @brief リスナーのパラメーターを設定
         * @param position 位置
         * @param forward 前
         * @param up 上
         */
        void SetListenerParameter(const Vector3& position, const Vector3& forward, const Vector3& up);

        /**
         * @brief
         * @param groupId グループの音量を設定
         * @param volume 音量
         */
        void SetGroupVolume(unsigned int groupId, float volume);
    };
}
