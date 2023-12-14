#pragma once
#include <bit>
#include <cstdint>
#include <algorithm>
#include <string>
#include <array>

namespace Glib
{
    template<std::integral T>
    constexpr T byteswap(T value) noexcept
    {
        static_assert(std::has_unique_object_representations_v<T>,
                      "T may not have padding bits");
        auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
        std::ranges::reverse(value_representation);
        return std::bit_cast<T>(value_representation);
    }

    /**
     * @brief �G���f�B�A���̃`�F�b�N
     * @return ���g���G���f�B�A�� : LE
     * @return �r�b�O�G���f�B�A�� : BE
     */
    inline std::string GetEndian()
    {
        if (std::endian::native == std::endian::little)
        {
            return "LE";
        }
        else if (std::endian::native == std::endian::big)
        {
            return "BE";
        }
    }
}
