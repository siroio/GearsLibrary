#pragma once
#include <string>
#include <vector>
#include <xaudio2.h>

namespace Glib
{
    class AudioClip
    {
    public:
        explicit AudioClip(
            std::string_view name,
            const WAVEFORMATEX& format,
            const std::vector<unsigned char>& data);

        /**
         * @brief 名前
         */
        std::string Name() const;

        /**
         * @brief フォーマット
         */
        const WAVEFORMATEX& Format() const;

        /**
         * @brief 音声データ
         */
        const std::vector<unsigned char>& RawData() const;

        /**
         * @brief バッファ
         */
        const XAUDIO2_BUFFER& Buffer() const;

        /**
         * @brief チャンネル数
         */
        unsigned int Channels() const;

        /**
         * @brief ループかどうか
         */
        bool Loop() const;

        /**
         * @brief ループの切り替え
         * @param enable
         */
        void Loop(bool enable);

        /**
         * @brief データをロード
         */
        bool Load(unsigned int id, std::string_view filePath);

    private:
        std::string clipName_{};
        WAVEFORMATEX format_{};
        XAUDIO2_BUFFER buffer_{};
        std::vector<unsigned char> audioData_;
    };
}
