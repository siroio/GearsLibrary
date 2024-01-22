#pragma once
#include <filesystem>

namespace Glib
{
    using FilePath = std::filesystem::path;

    /**
     * @brief ファイルパス文字列からパスへの変換
     * @param path ファイルパス
     */
    FilePath ConvertPath(std::string_view path)
    {
        return FilePath{ path };
    }

    /**
     * @brief 拡張子を取得
     * @param path ファイルパス
     * @param removePeriod 拡張子のドットを削除するか
     * @return 拡張子
     */
    std::string GetExtension(std::string_view path, bool removePeriod = true)
    {
        auto ext = std::filesystem::path{ path }.extension().generic_string();
        if (removePeriod) std::erase(ext, '.');
        return ext;
    }

    /**
     * @brief ファイルが存在しているか
     * @param path ファイルパス
     * @return
     */
    bool ExistsFile(std::string_view path)
    {
        return std::filesystem::exists(ConvertPath(path));
    }

    /**
     * @brief ファイルの拡張子をチェック
     * @param path ファイルパス
     * @param extension 拡張子
     */
    bool ExtensionCheck(std::string_view path, std::string_view extension)
    {
        auto ext = GetExtension(path);
        return extension.ends_with(ext);
    }
}
