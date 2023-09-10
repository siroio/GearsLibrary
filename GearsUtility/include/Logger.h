#pragma once
#include <iostream>
#include <format>
#include <string_view>
#include <TimeUtility.h>

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

    static inline bool DebugEnabled()
    {
#if defined(DEBUG) || defined(_DEBUG)
        return true;
#else
        return false;
#endif
    }

#if defined(DEBUG) || defined(_DEBUG)
    /**
     * @brief メッセージ指定なしassert
     */
    static void Assert(bool expression)
    {
        if (expression) return;
        Log("Assertion Error.", LogLevel::Error);
    }

    /**
     * @brief メッセージ指定ありassert
     */
    static void Assert(bool expression, std::string_view message)
    {
        if (expression) return;
        Log(message, LogLevel::Error);
    }

    /**
     * @brief メッセージ出力
     */
    static void Log(std::string_view message, LogLevel loglevel = LogLevel::Info)
    {
        std::cout << Glib::TimeUtility::CurrentTimeStr();
        switch (loglevel)
        {
            case LogLevel::Info: std::cout << "[INFO] " << message << std::endl; break;
            case LogLevel::Warn: std::cout << "[WARN] " << message << std::endl; break;
            case LogLevel::Error: std::cout << "[ERROR] " << message << std::endl; break;
        }
    }

    /**
     * @brief フォーマット指定で出力
     */
    template<class... Args>
    static void Format(std::string_view fmt, const Args&... args)
    {
        std::cout << Glib::TimeUtility::CurrentTimeStr();
        std::cout << std::format(fmt, args) << std::endl;
    }

#else
    static void Assert(...)
    {}
    static void Log(...)
    {}
    static void Format(...)
    {}
#endif
};
