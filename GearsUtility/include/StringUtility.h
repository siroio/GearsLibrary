#ifndef GEARS_STRING_UTILITY_H
#define GEARS_STRING_UTILITY_H

#include <string_view>
#include <string>
#include <vector>

namespace GLib
{
    /**
    * @brief 文字列を分割して返す
    * @param str 文字列
    * @param delim 区切り文字
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

    /**
     * @brief 指定したクラスの名前を取得
     * @return クラス名
     */
    template<class T>
    static constexpr std::string Nameof()
    {
        std::string name = typeid(T).name();
        std::size_t start = name.find(' ');
        if (start != std::string::npos) name = name.substr(start + 1);

        std::size_t end = name.rfind("::");
        if (end != std::string::npos) name = name.substr(end + 2);

        return name;
    }
};

#endif // !GEARS_STRING_UTILITY_H
