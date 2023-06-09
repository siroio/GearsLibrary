#ifndef GEARS_STRING_UTILITY_H
#define GEARS_STRING_UTILITY_H

#include <string_view>
#include <string>
#include <vector>

namespace GLib
{
    /**
    * @brief •¶š—ñ‚ğ•ªŠ„‚µ‚Ä•Ô‚·
    * @param str •¶š—ñ
    * @param delim ‹æØ‚è•¶š
    */
    static constexpr std::vector<std::string> Split(std::string_view str, char delim)
    {

        std::vector<std::string> tokens;
        auto begin = str.begin();
        auto end = str.end();
        tokens.reserve(std::count(str.begin(), str.end(), delim) + 1);

        while (begin != end)
        {
            auto pos = std::find(begin, end, delim);
            tokens.emplace_back(std::string_view(&(*begin), pos - begin));
            if (pos != end) begin = ++pos;
        }

        return tokens;
    }
};

#endif // !GEARS_STRING_UTILITY_H
