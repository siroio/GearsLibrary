#include "Debugger.h"

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
    Log("Assertion Error.", LogLevel::Error);
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
        case LogLevel::Info: std::cout << "[INFO] " << message << std::endl; break;
        case LogLevel::Warn: std::cout << "[WARN] " << message << std::endl; break;
        case LogLevel::Error: std::cout << "[ERROR] " << message << std::endl; break;
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