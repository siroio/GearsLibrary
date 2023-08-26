#pragma once
#include <string>
#include <Windows.h>
#include <functional>

namespace Glib
{
    /**
     * @brief メッセージボックス表示
    */
    class MsgBox final
    {
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

        static RESULT Show(std::string_view msg, std::string_view title, STYLE style, ICON icon, std::function<void(RESULT)> callback = nullptr);
        static RESULT Show(std::string_view msg, std::string_view title, std::function<void(RESULT)> callback);
        static RESULT Show(std::string_view msg, std::function<void(RESULT)> callback = nullptr);

    private:
        MsgBox() = delete;
        MsgBox(const MsgBox&) = delete;
        MsgBox& operator = (const MsgBox&) = delete;
    };
}
