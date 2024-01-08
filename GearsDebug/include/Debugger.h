#pragma once
#include <string_view>

namespace Glib
{
    enum class LogLevel
    {
        Info,
        Warn,
        Error
    };

    /**
     * @brief 簡易ロガークラス
     * @brief ファイル出力なし
     */
    class Debug final
    {
    private:
        Debug() = default;
        Debug(const Debug&) = delete;
        Debug& operator = (const Debug&) = delete;

    public:

        static bool Enabled();

        /**
         * @brief メッセージ指定なしassert
         */
        static void Assert(bool expression);

        /**
         * @brief メッセージ指定ありassert
         */
        static void Assert(bool expression, std::string_view message);

        /**
         * @brief メッセージ出力
         */
        static void Log(std::string_view message, LogLevel loglevel = LogLevel::Info);

        /**
         * @brief エラーメッセージ出力
         * @param message
        */
        static void Error(std::string_view message);

        /**
         * @brief 警告メッセージ出力
         * @param message
        */
        static void Warn(std::string_view message);

        /**
         * @brief レベルごとのPrefixを取得
         * @param loglevel
         */
        static std::string GetPrefix(LogLevel loglevel);
    };
}
