#pragma once
#include <string_view>
#include <memory>

namespace Glib
{
    class AudioClip;
}

namespace Glib::Internal::Interface
{
    /**
     * @brief 音声ローダー用インターフェース
     */
    class IAudioLoader
    {
    public:
        virtual ~IAudioLoader() = default;
        virtual std::shared_ptr<AudioClip> Load(std::string_view file) = 0;
    };
}
