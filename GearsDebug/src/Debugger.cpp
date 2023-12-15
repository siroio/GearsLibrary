#include <Debugger.h>
#include <cassert>

namespace
{
    constexpr char INFO_PREFIX[]{ "[INFO]" };
    constexpr char WARN_PREFIX[]{ "[WARN]" };
    constexpr char ERROR_PREFIX[]{ "[ERROR]" };
}

bool Glib::Debug::Enabled()
{
#if defined(DEBUG) || defined(_DEBUG)
    return true;
#else
    return false;
#endif
}

void Glib::Debug::Assert(bool expression)
{
#if defined(DEBUG) || defined(_DEBUG)
    if (expression) return;
    Log("Assertion Failed.", LogLevel::Error);
#endif
}

void Glib::Debug::Assert(bool expression, std::string_view message)
{
#if defined(DEBUG) || defined(_DEBUG)
    if (expression) return;
    Log(message, LogLevel::Error);
#endif
}

void Glib::Debug::Log(std::string_view message, LogLevel loglevel)
{
#if defined(DEBUG) || defined(_DEBUG)
    std::cout << Glib::TimeUtility::CurrentTimeStr();
    switch (loglevel)
    {
        case LogLevel::Info: std::cout << INFO_PREFIX << " " << message << std::endl; break;
        case LogLevel::Warn: std::cout << WARN_PREFIX << " " << message << std::endl; break;
        case LogLevel::Error: std::cout << ERROR_PREFIX << " " << message << std::endl; break;
    }
#endif
}

void Glib::Debug::Error(std::string_view message)
{
#if defined(DEBUG) || defined(_DEBUG)
    Log(message, LogLevel::Error);
#endif
}

void Glib::Debug::Warn(std::string_view message)
{
#if defined(DEBUG) || defined(_DEBUG)
    Log(message, LogLevel::Warn);
#endif
}
