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
     * @brief �������[�_�[�p�C���^�[�t�F�[�X
     */
    class IAudioLoader
    {
    public:
        virtual ~IAudioLoader() = default;
        virtual std::shared_ptr<AudioClip> Load(std::string_view file) = 0;
    };
}
