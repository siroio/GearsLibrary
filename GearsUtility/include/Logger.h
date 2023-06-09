#ifndef GEARS_LOGGER_H
#define GEARS_LOGGER_H

#include <cassert>
#include <string>
#include <string_view>
#include <iostream>

#include <TimeUtility.h>

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

    static inline void Assert(bool expression)
    {
#ifdef _DEBUG
        if (expression) return;
        Log("Assertion Error.", LogLevel::Error);
#endif
    }

    static inline void Assert(bool expression, std::string_view message)
    {
#ifdef _DEBUG
        if (expression) return;
        Log(message, LogLevel::Error);
#endif
    }

    static void Log(std::string_view message, LogLevel loglevel = LogLevel::Info)
    {
        auto time = GLib::TimeUtility::CurrentTime();
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

};

#endif // !GEARS_LOGGER_H
