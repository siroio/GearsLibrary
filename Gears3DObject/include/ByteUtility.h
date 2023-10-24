#pragma once
#include <algorithm>

namespace Glib
{
    inline void ByteSwap16(unsigned char* bytes) noexcept
    {
        std::swap(bytes[0], bytes[1]);
    }

    inline void ByteSwap32(unsigned char* bytes) noexcept
    {
        std::swap(bytes[0], bytes[3]);
        std::swap(bytes[1], bytes[2]);
    }

    inline void ByteSwap64(unsigned char* bytes) noexcept
    {
        std::swap(bytes[0], bytes[7]);
        std::swap(bytes[1], bytes[6]);
        std::swap(bytes[2], bytes[5]);
        std::swap(bytes[3], bytes[4]);
    }

    /**
     * @brief エンディアンのチェック
     * @return リトルエンディアン : LE
     * @return ビッグエンディアン : BE
     */
    inline std::string CheckEndian()
    {
        int check = 1;
        return *reinterpret_cast<char*>(&check) == 1 ? "LE" : "BE";
    }
}
