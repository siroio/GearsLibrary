#pragma once
#include <Internal/IAudioLoader.h>
#include <array>

namespace Glib::Internal::Audio
{
    class WavLoader : public Interface::IAudioLoader
    {
        struct CHUNK
        {
            std::array<char, 4> Id;
            unsigned int Size;
        };

        struct RIFF
        {
            CHUNK Chunk;
            std::array<char, 4> Type;
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
