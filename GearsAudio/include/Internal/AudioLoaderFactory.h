#pragma once
#include <Internal/IAudioLoader.h>

namespace Glib::Internal::Audio
{
    class AudioLoaderFactory
    {
    public:
        static std::unique_ptr<Interface::IAudioLoader> Create(std::string_view extension);
    };
}

