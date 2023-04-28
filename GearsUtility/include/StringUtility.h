#ifndef GEAR_STRING_UTILITY_H
#define GEAR_STRING_UTILITY_H

#include <string_view>
#include <string>
#include <vector>

/**
* @brief std::string�N���X�𑀍삷�邽�߂֗̕��N���X
*/
class StringUtility
{
public:
    /**
    * @brief ������𕪊����ĕԂ�
    * @param str �������镶����
    * @param delim �����Ɏg����؂蕶��
    */
    static std::vector<std::string> Split(std::string_view str, char delim)
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

#endif // !GEAR_STRING_UTILITY_H
