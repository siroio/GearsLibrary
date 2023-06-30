#pragma once
#ifdef _WIN32
#include <string>
#include <Windows.h>
#include <functional>
namespace Glib
{
    class MsgBox
    {
    private:
        MsgBox() = default;
        ~MsgBox() = default;

    public:
        enum class RESULT
        {
            OK,
            CANCEL,
            ABORT,
            RETRY,
            IGNORED,
            YES,
            NO,
            TRYAGAIN,
            CONTINUE
        };

        enum class STYLE
        {
            OK = MB_OK,
            OK_CANCEL = MB_OKCANCEL,
            ABORT_RETRY_IGNORE = MB_ABORTRETRYIGNORE,
            YES_NO_CANCEL = MB_YESNOCANCEL,
            YES_NO = MB_YESNO,
            RETRY_CANCEL = MB_RETRYCANCEL,
            CANCEL_TRY_CONTINUE = MB_CANCELTRYCONTINUE
        };

        enum class ICON
        {
            STOP_ICON = MB_ICONSTOP,
            ERROR_ICON = MB_ICONERROR,
            HAND_ICON = MB_ICONHAND,
            QUESTION_ICON = MB_ICONQUESTION,
            EXCLAMATION_ICON = MB_ICONEXCLAMATION,
            WARNING_ICON = MB_ICONWARNING,
            INFORMATION_ICON = MB_ICONINFORMATION,
            ASTERISK_ICON = MB_ICONASTERISK
        };

        static RESULT Show(const std::string& msg, const std::string& title, STYLE style, ICON icon, std::function<void(RESULT)> callback = nullptr);
        static RESULT Show(const std::string& msg, const std::string& title, std::function<void(RESULT)> callback);
        static RESULT Show(const std::string& msg, std::function<void(RESULT)> callback = nullptr);
    };
}

#endif // _WIN32
