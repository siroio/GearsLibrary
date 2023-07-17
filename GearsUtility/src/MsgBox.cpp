#include <MsgBox.h>

Glib::MsgBox::RESULT Glib::MsgBox::Show(const std::string& msg, const std::string& title, STYLE style, ICON icon, std::function<void(RESULT)> callback)
{
#ifdef UNICODE
    std::wstring msgStr = std::wstring{ msg.begin(), msg.end() };
    std::wstring titleStr = std::wstring{ title.begin(), title.end() };
#else
    const std::string& msgStr = msg;
    const std::string& titleStr = title;
#endif
    UINT uType = static_cast<UINT>(style) | static_cast<UINT>(icon);
    auto result = static_cast<RESULT>(MessageBox(
        nullptr,
        msgStr.c_str(),
        titleStr.c_str(),
        uType));
    if (callback) callback(result);
    return result;
}

Glib::MsgBox::RESULT Glib::MsgBox::Show(const std::string& msg, const std::string& title, std::function<void(RESULT)> callback)
{
    if (&title == nullptr) return Show(msg, "MessageBox", STYLE::OK, ICON::ASTERISK_ICON, callback);
    return Show(msg, title, STYLE::OK, ICON::ASTERISK_ICON, callback);
}

Glib::MsgBox::RESULT Glib::MsgBox::Show(const std::string& msg, std::function<void(RESULT)> callback)
{
    return Show(msg, "", STYLE::OK, ICON::ASTERISK_ICON, callback);
}
