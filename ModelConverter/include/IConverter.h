#pragma once
#include <string_view>

/**
 * @brief 変換用インターフェース
 */
class IConverter
{
public:
    IConverter() = default;
    virtual ~IConverter() = default;

    /**
     * @brief ファイルの読み込み
     * @param path ファイルへのパス
     * @return true : 成功
     * @return false : 失敗
     */
    virtual bool LoadFile(std::string_view path) = 0;

    /**
     * @brief ファイルの書き出し
     * @param path 拡張子含めたファイルパス
     * @return true : 成功
     * @return false : 失敗
     */
    virtual bool WriteFile(std::string_view path) = 0;
};

