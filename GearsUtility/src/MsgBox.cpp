#include <MsgBox.h>
#include <StringUtility.h>

Glib::MsgBox::RESULT Glib::MsgBox::Show(std::string_view msg, std::string_view title, STYLE style, ICON icon, std::function<void(RESULT)> callback)
{
#ifdef UNICODE
    std::wstring msgStr = Glib::StringToWide(msg);
    std::wstring titleStr = Glib::StringToWide(title);
#else
    std::string msgStr = ToString(msg);
    std::string titleStr = ToString(title);
#endif
    auto result = static_cast<RESULT>(MessageBox(
        nullptr,
        msgStr.c_str(),
        titleStr.c_str(),
        static_cast<UINT>(style) | static_cast<UINT>(icon)));
    if (callback) callback(result);
    return result;
}

Glib::MsgBox::RESULT Glib::MsgBox::Show(std::string_view msg, std::string_view title, std::function<void(RESULT)> callback)
{
    if (&title == nullptr) return Show(msg, "MessageBox", STYLE::OK, ICON::ASTERISK_ICON, callback);
    return Show(msg, title, STYLE::OK, ICON::ASTERISK_ICON, callback);
}

Glib::MsgBox::RESULT Glib::MsgBox::Show(std::string_view msg, std::function<void(RESULT)> callback)
{
    return Show(msg, "", STYLE::OK, ICON::ASTERISK_ICON, callback);
}

