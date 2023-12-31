#pragma once
#include <string>
#include <iosfwd>

/**
 * @brief ファイルのエンコード
 */
enum class Encode : unsigned char
{
    UTF8,
    UTF16
};

class FileUtility
{
public:
    /**
     * @brief 拡張子の取得
     * @param path ファイルパス
     * @param removePeriod 拡張子の[ . ]を削除するか
     * @return 拡張子
     */
    static std::string GetExtension(std::string_view path, bool removePeriod = true);

    /**
     * @brief バイナリへ書き込み
     * @param file
     * @param data
     * @param length
     */
    static void WriteToBinary(std::ofstream& file, const void* const data, const int length);

    /**
     * @brief バイナリから読み込み
     * @param file
     * @param data
     * @param length
     */
    static void ReadForBinary(std::ifstream& file, void* const data, const int length);

    /**
     * @brief [テキスト長 + テキスト本文]の形式をバッファへ読み込み
     * @param file ファイルを開いているストリーム
     * @param text バッファ
     * @param encode 読み込みエンコード
     */
    static void ReadText(std::ifstream& file, std::string& text, Encode encode = Encode::UTF8);
};
