#pragma once
#include <mutex>
#include <vector>
#include <string>
#include <string_view>

namespace Glib
{
    /**
     * @brief CSVロードクラス
     */
    class CSVLoader
    {
    private:
        using strings = std::vector<std::vector<std::string>>;
        std::string path;
        std::mutex mutex;

    public:
        /**
         * @brief ローダーの生成
         * @param path CSVのファイルパス
         */
        CSVLoader(std::string_view path);
        ~CSVLoader();
        /**
         * @brief CSVをロード
         * @return 2次元配列
         */
        strings Load() noexcept(false);
    };
}

