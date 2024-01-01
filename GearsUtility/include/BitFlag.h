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
        // enumの基底型: "char" "int" etc...
        using EnumType = std::underlying_type_t<Enum>;

        BitFlag() : value_{ EnumType{} }
        {}

        explicit BitFlag(Enum flag) : value_{ static_cast<EnumType>(flag) }
        {}

        /**
         * @brief フラグのセット
         * @param flag
         */
        void Set(Enum flag);

        /**
         * @brief フラグのセット
         * @param flag
         * @param enable
         */
        void Set(Enum flag, bool enable);

        /**
         * @brief フラグがセットされていれるか
         * @param flag
         * @return bool
         */
        bool Has(Enum flag) const;

        /**
         * @brief フラグのクリア
         * @param flag
         */
        void Clear(Enum flag);

        /**
         * @brief 値の取得
         */
        EnumType ToValue() const;

        /* 演算子 */

        BitFlag& operator |= (Enum&& flag);
        BitFlag& operator &= (Enum&& flag);
        BitFlag& operator ^= (Enum&& flag);
        BitFlag& operator | (Enum&& flag);
        BitFlag& operator & (Enum&& flag);
        BitFlag& operator ^ (Enum&& flag);
        bool operator==(const Enum& other) const;
        bool operator!=(const Enum& other) const;
        bool operator==(const BitFlag& other) const;
        bool operator!=(const BitFlag& other) const;

    private:
        EnumType value_;
    };

    template<class Enum> requires std::is_enum_v<Enum>
    void BitFlag<Enum>::Set(Enum flag)
    {
        value_ |= static_cast<EnumType>(flag);
    }

    template<class Enum> requires std::is_enum_v<Enum>
    void BitFlag<Enum>::Set(Enum flag, bool enable)
    {
        enable ? Set(flag) : Clear(flag);
    }

    template<class Enum> requires std::is_enum_v<Enum>
    bool BitFlag<Enum>::Has(Enum flag) const
    {
        return (value_ & static_cast<EnumType>(flag)) == 0;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    void BitFlag<Enum>::Clear(Enum flag)
    {
        value_ &= ~static_cast<EnumType>(flag);
    }

    template<class Enum> requires std::is_enum_v<Enum>
    inline BitFlag<Enum>::EnumType BitFlag<Enum>::ToValue() const
    {
        return value_;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    BitFlag<Enum>& BitFlag<Enum>::operator|=(Enum&& flag)
    {
        Set(flag);
        return *this;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    BitFlag<Enum>& BitFlag<Enum>::operator&=(Enum&& flag)
    {
        value_ &= static_cast<std::underlying_type_t<Enum>>(flag);
        return *this;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    BitFlag<Enum>& BitFlag<Enum>::operator^=(Enum&& flag)
    {
        value_ ^= static_cast<std::underlying_type_t<Enum>>(flag);
        return *this;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    BitFlag<Enum>& BitFlag<Enum>::operator|(Enum&& flag)
    {
        return BitFlag{ value_ } |= flag;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    BitFlag<Enum>& BitFlag<Enum>::operator&(Enum&& flag)
    {
        return BitFlag{ value_ } &= flag;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    BitFlag<Enum>& BitFlag<Enum>::operator^(Enum&& flag)
    {
        return BitFlag{ value_ } ^= flag;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    bool BitFlag<Enum>::operator==(const Enum& other) const
    {
        return (value_ & static_cast<EnumType>(other)) == 0;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    bool BitFlag<Enum>::operator!=(const Enum& other) const
    {
        return (value_ & static_cast<EnumType>(other)) != 0;;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    bool BitFlag<Enum>::operator==(const BitFlag& other) const
    {
        return value_ == other.value_;
    }

    template<class Enum> requires std::is_enum_v<Enum>
    bool BitFlag<Enum>::operator!=(const BitFlag& other) const
    {
        return value_ != other.value_;
    }
}

