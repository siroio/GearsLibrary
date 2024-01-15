#pragma once
#include <Internal/IAudioLoader.h>
#include <xaudio2.h>

namespace Glib::Internal::Audio
{
    class WavLoader : public Interface::IAudioLoader
    {
        struct CHUNK
        {
            char id[4];
            unsigned int size;
        };

        struct RIFF
        {
            CHUNK chunk;
            char type[4];
        };

    public:
        /**
         * @brief ファイルを開く
         * @param file wavファイルのパス
         * @return
         */
        std::shared_ptr<AudioClip> Load(std::string_view file) override;

    private:
        /**
         * @brief Wav内のチャンクを探索
         * @param stream Wavファイルのストリーム
         * @param name チャンク名
         * @return
         */
        static size_t FindChunk(std::ifstream& stream, std::string_view name);
    };
}
