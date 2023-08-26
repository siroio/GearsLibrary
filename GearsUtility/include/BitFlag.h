#pragma once
#include <concepts>
#include <bitset>
namespace Glib
{
    /**
     * @brief ビットフラグ
     */
    template<class Enum> requires std::is_enum_v<Enum>
    class BitFlag
    {
    public:
        using EnumType = std::underlying_type_t<Enum>;
        BitFlag() : value_{}
        {}

        explicit BitFlag(Enum flag) : value_{ static_cast<EnumType>(flag) }
        {}

        /**
         * @brief フラグのセット
         * @param flag
         */
        void Set(Enum flag);

        /**
         * @brief フラグがセットされていれるか
         * @param flag
         * @return bool
         */
        bool Set(Enum flag) const;

        /**
         * @brief フラグのクリア
         * @param flag
         */
        void Clear(Enum flag);

        /* 演算子 */

        BitFlag<Enum>& operator |= (Enum&& flag);
        BitFlag<Enum>& operator &= (Enum&& flag);
        BitFlag<Enum>& operator ^= (Enum&& flag);
        BitFlag<Enum>& operator | (Enum&& flag);
        BitFlag<Enum>& operator & (Enum&& flag);
        BitFlag<Enum>& operator ^ (Enum&& flag);
        bool operator==(const BitFlag<Enum>& other) const;

    private:
        EnumType value_;
    };

    template<class Enum>
    inline void BitFlag<Enum>::Set(Enum flag)
    {
        value_ |= static_cast<EnumType>(flag);
    }

    template<class Enum>
    inline bool BitFlag<Enum>::Set(Enum flag) const
    {
        return (value_ & static_cast<EnumType>(flag)) != 0;
    }

    template<class Enum>
    inline void BitFlag<Enum>::Clear(Enum flag)
    {
        value_ &= ~static_cast<EnumType>(flag);
    }

    template<class Enum>
    inline BitFlag<Enum>& BitFlag<Enum>::operator|=(Enum&& flag)
    {
        Set(flag);
        return *this;
    }

    template<class Enum>
    inline BitFlag<Enum>& BitFlag<Enum>::operator&=(Enum&& flag)
    {
        value_ &= static_cast<std::underlying_type_t<Enum>>(flag);
        return *this;
    }

    template<class Enum>
    inline BitFlag<Enum>& BitFlag<Enum>::operator^=(Enum&& flag)
    {
        value_ ^= static_cast<std::underlying_type_t<Enum>>(flag);
        return *this;
    }

    template<class Enum>
    inline BitFlag<Enum>& BitFlag<Enum>::operator|(Enum&& flag)
    {
        return BitFlag{ value_ } |= flag;
    }

    template<class Enum>
    inline BitFlag<Enum>& BitFlag<Enum>::operator&(Enum&& flag)
    {
        return BitFlag{ value_ } &= flag;
    }

    template<class Enum>
    inline BitFlag<Enum>& BitFlag<Enum>::operator^(Enum&& flag)
    {
        return BitFlag{ value_ } ^= flag;
    }

    template<class Enum>
    inline bool BitFlag<Enum>::operator==(const BitFlag<Enum>& other) const
    {
        return value_ == other.value_;
    }
}
