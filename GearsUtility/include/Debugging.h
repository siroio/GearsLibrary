#ifndef GEARS_DEBUGGING
#define GEARS_DEBUGGING

#ifdef _DEBUG

#include <cassert>
#include <string_view>

constexpr bool DEBUG_ENABLED{ true };

class Debug final
{
    enum class DebugLogLevel
    {
        Info,
        Warn,
        Error
    };

    static void Assert(bool expression);
    static void Assert(bool expression, const char* message);
    static void Assert(bool expression, std::string_view message);

    static void Log(const char* message);
    static void Log(std::string_view message);

    static void LogWarn(const char* message);
    static void LogWarn(std::string_view);

    static void LogError(const char* message);
    static void LogError(std::string_view);
};

#else

constexpr bool DEBUG_ENABLED{ false };
class Debug final
{
    enum class DebugLogLevel
    {
        Info,
        Warn,
        Error
    };

    static void Assert(bool expression)
    {}
    static void Assert(bool expression, const char* message)
    {}
    static void Assert(bool expression, std::string_view message)
    {}

    static void Log(const char* message)
    {}
    static void Log(std::string_view message)
    {}

    static void LogWarn(const char* message)
    {}
    static void LogWarn(std::string_view)
    {}

    static void LogError(const char* message)
    {}
    static void LogError(std::string_view)
    {}
};

#endif // _DEBUG

#endif // !GEARS_DEBUGGING
