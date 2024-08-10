#pragma once
#include <string_view>
#include <Singleton.h>

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
    class Debug final : public Singleton<Debug>
    {
    private:
        Debug() = default;
        friend Debug& Singleton<Debug>::Instance();
        virtual ~Debug() override;

    public:

        /**
         * @brief デバッグが有効か確認
         * @return 有効 : true
         * @return 無効 : false
         */
        static bool Enabled();

        /**
         * @brief コンソールへの出力を有効化
         * @brief 実行中に無効にはできません
         * @param enable 有効化
         * @param createConsole コンソールを作成するか
         */
        static void EnableConosleLog(bool enable, bool createConsole = true);

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
