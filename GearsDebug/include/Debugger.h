#include <iostream>
#include <format>
#include <string_view>
#include <TimeUtility.h>

namespace Glib
{
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
        enum class LogLevel
        {
            Info,
            Warn,
            Error
        };

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

#if defined(DEBUG) || defined(_DEBUG)
        /**
         * @brief フォーマット指定で出力
         */
        template<class... Args>
        static void Format(std::string_view fmt, const Args&... args)
        {
            std::cout << Glib::TimeUtility::CurrentTimeStr();
            std::cout << std::format(fmt, args) << std::endl;
        }
#endif
    };
}