#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <cstdint>
#include <Windows.h>

namespace Glib
{
    /**
     * @brief 文字列変換用コードページ
     */
    enum class CodePage : unsigned int
    {
        IBM_EBCDIC_US_Canada = 37,
        OEM_US = 437,
        IBM_EBCDIC_International = 500,
        Arabic_ASMO_708 = 708,
        Arabic_DOS = 720,
        Greek_DOS = 737,
        Baltic_DOS = 775,
        OEM_Western_Europe = 850,
        OEM_Central_Europe = 852,
        OEM_Cyrillic = 855,
        Turkish_DOS = 857,
        OEM_Multilingual_Latin1 = 858,
        Portuguese_DOS = 860,
        Icelandic_DOS = 861,
        Hebrew_DOS = 862,
        French_Canada_DOS = 863,
        Arabic_864 = 864,
        Nordic_DOS = 865,
        Cyrillic_DOS = 866,
        Greek_Modern_DOS = 869,
        IBM_EBCDIC_Multilingual_Latin2 = 870,
        Thai_Windows = 874,
        IBM_EBCDIC_Greek_Modern = 875,
        Japanese_Shift_JIS = 932,
        Simplified_Chinese_GB2312 = 936,
        Korean = 949,
        Traditional_Chinese_Big5 = 950,
        IBM_Latin1 = 1047,
        IBM_EBCDIC_US_Canada_Europe = 1140,
        IBM_EBCDIC_Germany_Europe = 1141,
        IBM_EBCDIC_Denmark_Norway_Europe = 1142,
        IBM_EBCDIC_Finland_Sweden_Europe = 1143,
        IBM_EBCDIC_Italy_Europe = 1144,
        IBM_EBCDIC_Spain_Europe = 1145,
        IBM_EBCDIC_UK_Europe = 1146,
        IBM_EBCDIC_France_Europe = 1147,
        IBM_EBCDIC_International_Europe = 1148,
        IBM_EBCDIC_Icelandic_Europe = 1149,
        Unicode_UTF16LE = 1200,
        Unicode_UTF16BE = 1201,
        Central_Europe_Windows = 1250,
        Cyrillic_Windows = 1251,
        Western_Europe_Windows = 1252,
        Greek_Windows = 1253,
        Turkish_Windows = 1254,
        Hebrew_Windows = 1255,
        Arabic_Windows = 1256,
        Baltic_Windows = 1257,
        Vietnamese_Windows = 1258,
        Korean_Johab = 1361,
        Western_Europe_Mac = 10000,
        Japanese_Mac = 10001,
        Traditional_Chinese_Mac = 10002,
        Korean_Mac = 10003,
        Arabic_Mac = 10004,
        Hebrew_Mac = 10005,
        Greek_Mac = 10006,
        Cyrillic_Mac = 10007,
        Simplified_Chinese_Mac = 10008,
        Romanian_Mac = 10010,
        Ukrainian_Mac = 10017,
        Thai_Mac = 10021,
        Central_Europe_Mac = 10029,
        Icelandic_Mac = 10079,
        Turkish_Mac = 10081,
        Croatian_Mac = 10082,
        Simplified_Chinese_CNS = 20000,
        Taiwanese_TCA = 20001,
        Simplified_Chinese_Eten = 20002,
        IBM_Taiwan = 20003,
        TeleText_Taiwan = 20004,
        Wang_Taiwan = 20005,
        Western_Europe_IA5 = 20105,
        German_IA5 = 20106,
        Swedish_IA5 = 20107,
        Norwegian_IA5 = 20108,
        US_ASCII = 20127,
        T_61 = 20261,
        ISO_6937 = 20269,
        IBM_EBCDIC_Germany = 20273,
        IBM_EBCDIC_Denmark_Norway = 20277,
        IBM_EBCDIC_Finland_Sweden = 20278,
        IBM_EBCDIC_Italy = 20280,
        IBM_EBCDIC_Spain = 20284,
        IBM_EBCDIC_UK = 20285,
        IBM_EBCDIC_Japanese_Katakana = 20290,
        IBM_EBCDIC_France = 20297,
        IBM_EBCDIC_Arabic = 20420,
        IBM_EBCDIC_Greek = 20423,
        IBM_EBCDIC_Hebrew = 20424,
        IBM_EBCDIC_Korean_Extended = 20833,
        IBM_EBCDIC_Thai = 20838,
        Cyrillic_KOI8_R = 20866,
        IBM_EBCDIC_Icelandic = 20871,
        IBM_EBCDIC_Cyrillic_Russian = 20880,
        IBM_EBCDIC_Turkish = 20905,
        IBM_Latin = 20924,
        Japanese_JIS = 20932,
        Simplified_Chinese_GB2312_80 = 20936,
        Korean_Wansung = 20949,
        IBM_EBCDIC_Cyrillic_Serbian_Bulgarian = 21025,
        Lowercase_Extended_Alphabet = 21027,
        Cyrillic_KOI8_U = 21866,
        Western_Europe_ISO = 28591,
        Central_Europe_ISO = 28592,
        Latin_3_ISO = 28593,
        Baltic_ISO = 28594,
        Cyrillic_ISO = 28595,
        Arabic_ISO = 28596,
        Greek_ISO = 28597,
        Hebrew_ISO_Visual = 28598,
        Turkish_ISO = 28599,
        Lithuanian_ISO = 28603,
        Latin_9_ISO = 28605,
        Europe_ISO = 29001,
        Hebrew_ISO_Logical = 38598,
        User_Defined = 50000,
        Automatic_Selection = 50001,
        Japanese_JIS_X_0208 = 50220,
        Japanese_JIS_X_0201 = 50221,
        Japanese_JIS_X_0201_SO_SI = 50222,
        Korean_ISO = 50227,
        Simplified_Chinese_ISO_2022 = 50229,
        IBM_Japanese = 50930,
        IBM_Japanese_US_Canada = 50931,
        Japanese_Automatic_Selection = 50932,
        IBM_Korean_Extended = 50933,
        IBM_Simplified_Chinese = 50935,
        Simplified_Chinese_Automatic_Selection = 50936,
        IBM_Traditional_Chinese = 50937,
        Japanese_Katakana_From_JIS_X_0213 = 50939,
        Korean_Automatic_Selection = 50949,
        Traditional_Chinese_Automatic_Selection = 50950,
        Cyrillic_Automatic_Selection = 51251,
        Greek_Automatic_Selection = 51253,
        Arabic_Automatic_Selection = 51256,
        Japanese_EUC = 51932,
        Simplified_Chinese_EUC = 51936,
        Korean_EUC = 51949,
        Simplified_Chinese_HZ = 52936,
        Simplified_Chinese_GB18030 = 54936,
        ISCII_Devanagari = 57002,
        ISCII_Bengali = 57003,
        ISCII_Tamil = 57004,
        ISCII_Telugu = 57005,
        ISCII_Assamese = 57006,
        ISCII_Oriya = 57007,
        ISCII_Kannada = 57008,
        ISCII_Malayalam = 57009,
        ISCII_Gujarati = 57010,
        ISCII_Punjabi = 57011,
        Unicode_UTF_7 = 65000,
        Unicode_UTF_8 = 65001
    };

    /**
    * @brief 文字列を分割して返す
    * @param str 文字列
    * @param delim 区切り文字
    */
    static constexpr std::vector<std::string> Split(std::string_view str, char delim)
    {
        std::vector<std::string> tokens;
        auto begin = str.begin();
        auto end = str.end();
        tokens.reserve(std::count(str.begin(), str.end(), delim) + 1);

        while (begin != end)
        {
            auto pos = std::find(begin, end, delim);
            tokens.emplace_back(std::string_view(&(*begin), pos - begin));
            if (pos != end) begin = ++pos;
        }

        return tokens;
    }

    /**
     * @brief 指定したクラスの名前を取得
     * @return クラス名
     */
    template<class T>
    static constexpr std::string nameof(const T& object)
    {
        std::string name = typeid(object).name();
        std::size_t start = name.find(' ');
        if (start != std::string::npos) name = name.substr(start + 1);

        std::size_t end = name.rfind("::");
        if (end != std::string::npos) name = name.substr(end + 2);

        return name;
    }

    /**
     * @brief 指定したクラスの名前を取得
     * @return クラス名
     */
    template<class T>
    static constexpr std::string nameof()
    {
        std::string name = typeid(T).name();
        std::size_t start = name.find(' ');
        if (start != std::string::npos) name = name.substr(start + 1);

        std::size_t end = name.rfind("::");
        if (end != std::string::npos) name = name.substr(end + 2);

        return name;
    }

    /**
     * @brief stringをコピー
     */
    static constexpr std::string ToString(std::string_view sv)
    {
        return std::string{ sv.begin(), sv.end() };
    }

    /**
     * @brief マルチバイト文字列に変換
     */
    static inline std::wstring StringToWide(std::string_view sv)
    {
        int size = MultiByteToWideChar(
            CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            sv.data(),
            -1,
            nullptr,
            0);

        if (size == 0) return L"";

        std::wstring wstr{};
        wstr.resize(static_cast<size_t>(size) - 1);
        MultiByteToWideChar(
            CP_ACP,
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            sv.data(),
            -1,
            wstr.data(),
            size);

        return wstr;
    }

    /**
     * @brief 文字列に変換
     */
    static inline std::string WideToString(std::wstring_view sv)
    {
        int bufferSize = WideCharToMultiByte(CP_ACP, 0, &sv[0], -1, nullptr, 0, nullptr, nullptr);
        if (bufferSize == 0) return "";

        std::string result(bufferSize, '\0');
        return WideCharToMultiByte(CP_ACP, 0, &sv[0], -1, result.data(), bufferSize, nullptr, nullptr) == 0 ?
            "" : result;
    }

    /**
     * @brief 文字コードの変換
     * @param text 対象文字列
     * @param from 変換元コードページ
     * @param to 変換先コードページ
     * @return 変換後文字列
     */
    static inline std::string CharConv(std::string_view text, CodePage from = CodePage::Japanese_Shift_JIS, CodePage to = CodePage::Unicode_UTF_8)
    {
        // ワイド文字列へ変換
        int wideSize = MultiByteToWideChar(static_cast<UINT>(from), 0, text.data(), -1, nullptr, 0);
        if (wideSize == 0) return "";

        std::wstring wideStr(wideSize, L'\0');
        MultiByteToWideChar(static_cast<UINT>(from), 0, text.data(), -1, wideStr.data(), wideSize);


        int convSize = WideCharToMultiByte(static_cast<UINT>(to), 0, wideStr.data(), -1, nullptr, 0, nullptr, nullptr);
        if (convSize == 0) return "";

        std::string convStr(convSize, '\0');
        WideCharToMultiByte(static_cast<UINT>(to), 0, wideStr.data(), -1, convStr.data(), convSize, nullptr, nullptr);

        return convStr;
    }
};
