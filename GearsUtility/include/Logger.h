#ifndef GEARS_LOGGER_H
#define GEARS_LOGGER_H

#include <cassert>
#include <string>
#include <string_view>
#include <iostream>

#include <TimeUtility.h>
#include <source_location>
#include <cstdarg>

class Debug final
{
private:
    Debug() = default;

public:
    enum class LogLevel
    {
        Info,
        Warn,
        Error
    };

    static inline bool DebugEnabled()
    {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif
    }

#ifdef _DEBUG

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
        const auto time = GLib::TimeUtility::CurrentTime();
        std::cout
            << "[" << std::setfill('0') << std::setw(2) << time.hours
            << ":" << std::setfill('0') << std::setw(2) << time.minutes
            << ":" << std::setfill('0') << std::setw(2) << time.seconds << "]";

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
    static void format(const char* format, ...)
    {
        const auto time = GLib::TimeUtility::CurrentTime();
        std::cout
            << "[" << std::setfill('0') << std::setw(2) << time.hours
            << ":" << std::setfill('0') << std::setw(2) << time.minutes
            << ":" << std::setfill('0') << std::setw(2) << time.seconds << "]";

        va_list valist;
        va_start(valist, format);
        vprintf(format, valist);
        va_end(valist);
    }
#else
    static void Assert(...)
    {}
    static void Log(...)
    {}
    static void format(...)
    {}
#endif

};

#endif // !GEARS_LOGGER_H
