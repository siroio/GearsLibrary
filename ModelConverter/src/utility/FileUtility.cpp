#include <utility/FileUtility.h>
#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <string>

namespace
{
    constexpr unsigned int SJIS_CODEPAGE = 932;
}

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
    ReadText(file, text, bufLength, encode);
}

void FileUtility::ReadText(std::ifstream& file, std::string& text, int length, Encode encode)
{
    text = "";
    if (length < 1) return;

    if (encode == Encode::UTF16)
    {
        std::wstring textBuf(length / sizeof(wchar_t), L'\0');
        ReadForBinary(file, &textBuf[0], length);

        // 読み込んだ文字列をwstringからstringに変換
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, textBuf.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (bufferSize == 0) return;

        std::string result(bufferSize, '\0');
        if (WideCharToMultiByte(CP_UTF8, 0, textBuf.data(), -1, result.data(), bufferSize, nullptr, nullptr) == 0)
        {
            return;
        }

        result.resize(std::char_traits<char>::length(result.data()));
        result.shrink_to_fit();
        text = std::move(result);
    }
    else if (encode == Encode::UTF8)
    {
        std::string textBuf(length, '\0');
        file.read(textBuf.data(), length);
        textBuf = std::string(textBuf.c_str());
        textBuf.resize(std::char_traits<char>::length(textBuf.data()));
        textBuf.shrink_to_fit();
        textBuf = ToUTF8(textBuf);
        text = std::move(textBuf);
    }
}

std::string FileUtility::ToUTF8(std::string_view text)
{
    // ワイド文字列へ変換
    int wideSize = MultiByteToWideChar(SJIS_CODEPAGE, 0, text.data(), -1, nullptr, 0);
    if (wideSize == 0) return "";

    std::wstring wideStr(wideSize, L'\0');
    MultiByteToWideChar(SJIS_CODEPAGE, 0, text.data(), -1, wideStr.data(), wideSize);


    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Size == 0) return "";

    std::string utf8Str(utf8Size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size, nullptr, nullptr);

    return utf8Str;
}
