#include <Debugger.h>
#include <cassert>

#if defined(DEBUG) || defined(_DEBUG)

namespace
{
    constexpr char INFO_PREFIX[]{ "[INFO]" };
    constexpr char WARN_PREFIX[]{ "[WARN]" };
    constexpr char ERROR_PREFIX[]{ "[ERROR]" };
}

bool Glib::Debug::Enabled()
{
    return true;
}

void Glib::Debug::Assert(bool expression)
{
    if (expression) return;
    Log("Assertion Failed.", LogLevel::Error);
}

void Glib::Debug::Assert(bool expression, std::string_view message)
{
    if (expression) return;
    Log(message, LogLevel::Error);
}

void Glib::Debug::Log(std::string_view message, LogLevel loglevel)
{
    std::cout
        << Glib::TimeUtility::CurrentTimeStr()
        << GetPrefix(loglevel) << " "
        << message << "\n";
}

void Glib::Debug::Error(std::string_view message)
{
    Log(message, LogLevel::Error);
}

void Glib::Debug::Warn(std::string_view message)
{
    Log(message, LogLevel::Warn);
}

std::string Glib::Debug::GetPrefix(LogLevel loglevel)
{
    switch (loglevel)
    {
        case LogLevel::Info: return INFO_PREFIX;
        case LogLevel::Warn: return WARN_PREFIX;
        case LogLevel::Error: return ERROR_PREFIX;
        default: return "[NONE]";
    }
}

#else

bool Glib::Debug::Enabled()
{
    return false;
}

void Glib::Debug::Assert(bool expression)
{}

void Glib::Debug::Assert(bool expression, std::string_view message)
{}

void Glib::Debug::Log(std::string_view message, LogLevel loglevel)
{}

void Glib::Debug::Error(std::string_view message)
{}

void Glib::Debug::Warn(std::string_view message)
{}

std::string Glib::Debug::GetPrefix(LogLevel loglevel)
{}

#endif
