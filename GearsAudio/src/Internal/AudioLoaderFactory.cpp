#include <Internal/AudioLoaderFactory.h>
#include <Internal/WavLoader.h>

std::unique_ptr<Glib::Internal::Interface::IAudioLoader> Glib::Internal::Audio::AudioLoaderFactory::Create(std::string_view extension)
{
    if (extension == "wav") return std::make_unique<WavLoader>();
    if (extension == "ogg") return nullptr;
    return nullptr;
}
