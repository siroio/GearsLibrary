#pragma once
#include <string_view>
#include <string>
#include <vector>
#include <Windows.h>

namespace Glib
{
    /**
    * @brief ������𕪊����ĕԂ�
    * @param str ������
    * @param delim ��؂蕶��
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
     * @brief �w�肵���N���X�̖��O���擾
     * @return �N���X��
     */
    template<class T>
    static constexpr std::string nameof()
    {
        std::string name = typeid(T).name();
        std::size_t start = name.find(' ');
        if (start != std::string::npos) name = name.substr(start + 1);

        std::size_t end = name.rfind("::");
        if (end != std::string::npos) name = name.substr(end + 2);

        return name;
    }

    /**
     * @brief string���R�s�[
     */
    static constexpr std::string ToString(std::string_view sv)
    {
        return { sv.begin(), sv.end() };
    }

    /**
     * @brief �}���`�o�C�g������ɕϊ�
     */
    static inline std::wstring StringToWide(std::string_view sv)
    {
        int size = MultiByteToWideChar(
            CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            sv.data(),
            -1,
            nullptr,
            0);

        if (size == 0) return L"";

        std::wstring wstr{};
        wstr.resize(size - 1);
        MultiByteToWideChar(
            CP_ACP,
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            sv.data(),
            -1,
            wstr.data(),
            size);

        return wstr;
    }
};
