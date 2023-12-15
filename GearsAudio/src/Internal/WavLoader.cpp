#include <Internal/WavLoader.h>
#include <AudioClip.h>
#include <xaudio2.h>
#include <fstream>
#include <filesystem>

std::shared_ptr<Glib::AudioClip> Glib::Internal::Audio::WavLoader::Load(std::string_view file)
{
    const std::filesystem::path wavPath = std::filesystem::absolute(file);
    if (exists(wavPath) || wavPath.extension().generic_string().ends_with("wav")) return nullptr;
    std::ifstream wavFile{ file.data(), std::ios::in, std::ios::binary };

    // 開けるか確認
    if (!wavFile.is_open()) return nullptr;

    // riffチャンクの取得
    RIFF riff{};
    wavFile.read(reinterpret_cast<char*>(&riff), sizeof(riff));

    // fmtチャンクの取得
    const auto size = FindChunk(wavFile, "fmt ");
    WAVEFORMATEX format{};

    if (size == 16 || size == 18)
    {
        wavFile.read(reinterpret_cast<char*>(&format), size);
    }
    else if (size == 40)
    {
        wavFile.read(reinterpret_cast<char*>(&format), sizeof(format));
        wavFile.seekg(static_cast<std::streamoff>(size) - sizeof(format), std::ios::cur);
    }

    // データサイズ取得
    const size_t dataSize = FindChunk(wavFile, "data ");
    if (dataSize <= 0) return nullptr;
    const auto dataBuffer = std::vector<BYTE>(dataSize);
    wavFile.read(reinterpret_cast<char*>(dataBuffer.front()), dataSize);
    wavFile.close();
    auto audioClip = std::make_shared<AudioClip>(
        wavPath.filename().generic_string(),
        format,
        dataBuffer
    );
    return audioClip;
}

size_t Glib::Internal::Audio::WavLoader::FindChunk(std::ifstream& stream, std::string_view name)
{
    while (!stream.eof())
    {
        CHUNK chunk{};
        stream.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
        std::string chunkName{ chunk.Id.data(), 4 };
        if (chunkName == name) return chunk.Size;
        stream.seekg(chunk.Size, std::ios::cur);
    }
    return 0;
}
