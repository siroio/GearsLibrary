#include <Debugger.h>
#include <Internal/ImGuiManager.h>
#include <format>
#include <TimeUtility.h>

#if defined(DEBUG) || defined(_DEBUG)

namespace
{
    constexpr char INFO_PREFIX[]{ "[INFO]" };
    constexpr char WARN_PREFIX[]{ "[WARN]" };
    constexpr char ERROR_PREFIX[]{ "[ERROR]" };
    bool s_enableConsole{ false };
    auto s_imgui = Glib::Internal::Debug::ImGuiManager::Instance();
}

Glib::Debug::~Debug()
{
    FreeConsole();
}

bool Glib::Debug::Enabled()
{
    return true;
}

void Glib::Debug::EnableConosleLog(bool enable, bool createConsole)
{
    s_enableConsole = enable;
    if (!enable && s_enableConsole) return;

    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AllocConsole();
    }
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1024);
    FILE* output;
    FILE* error;
    freopen_s(&output, "CONOUT$", "w", stdout);
    freopen_s(&error, "CONOUT$", "w", stderr);
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
    const std::string message_formatted{
        std::format("{}{} {}\n", TimeUtility::CurrentTimeStr(), GetPrefix(loglevel), message)
    };

    if (s_enableConsole) std::printf("%s", message_formatted.data());
    s_imgui->Log(message_formatted, loglevel);
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

void Glib::Debug::EnableConosleLog(bool enable, bool createConsole)
{}

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
{
    return "";
}

#endif
