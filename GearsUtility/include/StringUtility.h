#pragma once
#include <string>
#include <sstream>
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
        IBMEbcdicUsCanada = 37,
        OEMUs = 437,
        IBMEbcdicInternational = 500,
        ArabicAsmo708 = 708,
        ArabicDos = 720,
        GreekDos = 737,
        BalticDos = 775,
        OEMWesternEurope = 850,
        OEMCentralEurope = 852,
        OEMCyrillic = 855,
        TurkishDos = 857,
        OEMMultilingualLatin1 = 858,
        PortugueseDos = 860,
        IcelandicDos = 861,
        HebrewDos = 862,
        FrenchCanadaDos = 863,
        Arabic864 = 864,
        NordicDos = 865,
        CyrillicDos = 866,
        GreekModernDos = 869,
        IBMEbcdicMultilingualLatin2 = 870,
        ThaiWindows = 874,
        IBMEbcdicGreekModern = 875,
        JapaneseShiftJis = 932,
        SimplifiedChineseGb2312 = 936,
        Korean = 949,
        TraditionalChineseBig5 = 950,
        IBMLatin1 = 1047,
        IBMEbcdicUsCanadaEurope = 1140,
        IBMEbcdicGermanyEurope = 1141,
        IBMEbcdicDenmarkNorwayEurope = 1142,
        IBMEbcdicFinlandSwedenEurope = 1143,
        IBMEbcdicItalyEurope = 1144,
        IBMEbcdicSpainEurope = 1145,
        IBMEbcdicUkEurope = 1146,
        IBMEbcdicFranceEurope = 1147,
        IBMEbcdicInternationalEurope = 1148,
        IBMEbcdicIcelandicEurope = 1149,
        UnicodeUtf16Le = 1200,
        UnicodeUtf16Be = 1201,
        CentralEuropeWindows = 1250,
        CyrillicWindows = 1251,
        WesternEuropeWindows = 1252,
        GreekWindows = 1253,
        TurkishWindows = 1254,
        HebrewWindows = 1255,
        ArabicWindows = 1256,
        BalticWindows = 1257,
        VietnameseWindows = 1258,
        KoreanJohab = 1361,
        WesternEuropeMac = 10000,
        JapaneseMac = 10001,
        TraditionalChineseMac = 10002,
        KoreanMac = 10003,
        ArabicMac = 10004,
        HebrewMac = 10005,
        GreekMac = 10006,
        CyrillicMac = 10007,
        SimplifiedChineseMac = 10008,
        RomanianMac = 10010,
        UkrainianMac = 10017,
        ThaiMac = 10021,
        CentralEuropeMac = 10029,
        IcelandicMac = 10079,
        TurkishMac = 10081,
        CroatianMac = 10082,
        SimplifiedChineseCns = 20000,
        TaiwaneseTca = 20001,
        SimplifiedChineseEten = 20002,
        IBMTaiwan = 20003,
        TeleTextTaiwan = 20004,
        WangTaiwan = 20005,
        WesternEuropeIa5 = 20105,
        GermanIa5 = 20106,
        SwedishIa5 = 20107,
        NorwegianIa5 = 20108,
        USASCII = 20127,
        T61 = 20261,
        ISO6937 = 20269,
        IBMEbcdicGermany = 20273,
        IBMEbcdicDenmarkNorway = 20277,
        IBMEbcdicFinlandSweden = 20278,
        IBMEbcdicItaly = 20280,
        IBMEbcdicSpain = 20284,
        IBMEbcdicUk = 20285,
        IBMEbcdicJapaneseKatakana = 20290,
        IBMEbcdicFrance = 20297,
        IBMEbcdicArabic = 20420,
        IBMEbcdicGreek = 20423,
        IBMEbcdicHebrew = 20424,
        IBMEbcdicKoreanExtended = 20833,
        IBMEbcdicThai = 20838,
        CyrillicKOI8R = 20866,
        IBMEbcdicIcelandic = 20871,
        IBMEbcdicCyrillicRussian = 20880,
        IBMEbcdicTurkish = 20905,
        IBMLatin = 20924,
        JapaneseJIS = 20932,
        SimplifiedChineseGb231280 = 20936,
        KoreanWansung = 20949,
        IBMEbcdicCyrillicSerbianBulgarian = 21025,
        LowercaseExtendedAlphabet = 21027,
        CyrillicKOI8U = 21866,
        WesternEuropeISO = 28591,
        CentralEuropeISO = 28592,
        Latin3ISO = 28593,
        BalticISO = 28594,
        CyrillicISO = 28595,
        ArabicISO = 28596,
        GreekISO = 28597,
        HebrewISOVisual = 28598,
        TurkishISO = 28599,
        LithuanianISO = 28603,
        Latin9ISO = 28605,
        EuropeISO = 29001,
        HebrewISOLogical = 38598,
        UserDefined = 50000,
        AutomaticSelection = 50001,
        JapaneseJISX0208 = 50220,
        JapaneseJISX0201 = 50221,
        JapaneseJISX0201SOSI = 50222,
        KoreanISO = 50227,
        SimplifiedChineseISO2022 = 50229,
        IBMJapanese = 50930,
        IBMJapaneseUSCanada = 50931,
        JapaneseAutomaticSelection = 50932,
        IBMKoreanExtended = 50933,
        IBMSimplifiedChinese = 50935,
        SimplifiedChineseAutomaticSelection = 50936,
        IBMTraditionalChinese = 50937,
        JapaneseKatakanaFromJISX0213 = 50939,
        KoreanAutomaticSelection = 50949,
        TraditionalChineseAutomaticSelection = 50950,
        CyrillicAutomaticSelection = 51251,
        GreekAutomaticSelection = 51253,
        ArabicAutomaticSelection = 51256,
        JapaneseEUC = 51932,
        SimplifiedChineseEUC = 51936,
        KoreanEUC = 51949,
        SimplifiedChineseHZ = 52936,
        SimplifiedChineseGB18030 = 54936,
        ISCIIDevanagari = 57002,
        ISCIIBengali = 57003,
        ISCIITamil = 57004,
        ISCIITelugu = 57005,
        ISCIIAssamese = 57006,
        ISCIIOriya = 57007,
        ISCIIKannada = 57008,
        ISCIIMalayalam = 57009,
        ISCIIGujarati = 57010,
        ISCIIPunjabi = 57011,
        UnicodeUTF7 = 65000,
        UnicodeUTF8 = 65001
    };

    /**
    * @brief 文字列を分割して返す
    * @param str 文字列
    * @param delim 区切り文字
    */
    static std::vector<std::string> Split(std::string_view str, char delim)
    {
        std::vector<std::string> tokens;
        std::istringstream iss{ str.data() };
        std::string buf;

        while (std::getline(iss, buf, delim))
        {
            tokens.push_back(buf);
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
    static inline std::string CharConv(std::string_view text, CodePage from = CodePage::JapaneseShiftJis, CodePage to = CodePage::UnicodeUTF8)
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
