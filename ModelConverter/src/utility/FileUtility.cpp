#include <utility/FileUtility.h>
#include <filesystem>
#include <fstream>
#include <Windows.h>

std::string FileUtility::GetExtension(std::string_view path, bool removePeriod)
{
    auto ext = std::filesystem::path{ path }.extension().generic_string();
    if (removePeriod) std::erase(ext, '.');
    return ext;
}

void FileUtility::WriteToBinary(std::ofstream& file, const void* const data, const int length)
{
    file.write(reinterpret_cast<const char*>(data), length);
}

void FileUtility::ReadForBinary(std::ifstream& file, void* const data, const int length)
{
    file.read(reinterpret_cast<char*>(data), length);
}

void FileUtility::ReadText(std::ifstream& file, std::string& text, Encode encode)
{
    int bufLength{ 0 };
    ReadForBinary(file, &bufLength, 4);
    text = "";
    if (bufLength < 1) return;

    if (encode == Encode::UTF16)
    {
        std::wstring textBuf(bufLength / sizeof(wchar_t), L'\0');
        ReadForBinary(file, &textBuf[0], bufLength);

        // 読み込んだ文字列をwstringからstringに変換
        int bufferSize = WideCharToMultiByte(CP_ACP, 0, textBuf.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (bufferSize == 0) return;

        std::string result(bufferSize, '\0');
        if (WideCharToMultiByte(CP_ACP, 0, textBuf.data(), -1, result.data(), bufferSize, nullptr, nullptr) == 0)
        {
            return;
        }

        text = std::move(result);
    }
    else if (encode == Encode::UTF8)
    {
        std::string textBuf(bufLength, '\0');
        file.read(textBuf.data(), bufLength);
        text = std::move(textBuf);
    }
}

