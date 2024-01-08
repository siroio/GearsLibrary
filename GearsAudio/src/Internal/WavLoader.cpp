#include <Internal/WavLoader.h>
#include <AudioClip.h>
#include <xaudio2.h>
#include <fstream>
#include <filesystem>

std::shared_ptr<Glib::AudioClip> Glib::Internal::Audio::WavLoader::Load(std::string_view file)
{
    const auto wavPath = std::filesystem::absolute(file);
    if (!(std::filesystem::exists(wavPath) && wavPath.extension().string().ends_with("wav"))) return nullptr;
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
    const size_t dataSize = FindChunk(wavFile, "data");
    if (dataSize <= 0) return nullptr;

    auto dataBuffer = std::vector<BYTE>(dataSize);
    wavFile.read(reinterpret_cast<char*>(dataBuffer.data()), dataSize);
    auto audioClip = std::make_shared<AudioClip>(
        wavPath.filename().string(),
        format,
        dataBuffer
    );
    return audioClip;
}

size_t Glib::Internal::Audio::WavLoader::FindChunk(std::ifstream& stream, std::string_view name)
{
    while (true)
    {
        CHUNK chunk{};
        stream.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
        std::string chunkStr{ chunk.Id, 4 };
        if (chunkStr.starts_with(name)) return chunk.Size;
        else stream.seekg(chunk.Size, std::ios_base::cur);
    }
    return 0;
}
