#pragma once
#include <concepts>
#include <bitset>
namespace Glib
{
    /**
     * @brief �r�b�g�t���O
     */
    template<class Enum> requires std::is_enum_v<Enum>
    class BitFlag
    {
    public:
        // enum�̊��^: "char" "int" etc...
        using EnumType = std::underlying_type_t<Enum>;

        BitFlag() : value_{ EnumType{} }
        {}

        explicit BitFlag(Enum flag) : value_{ static_cast<EnumType>(flag) }
        {}

        /**
         * @brief �t���O�̃Z�b�g
         * @param flag
         */
        void Set(Enum flag);

        /**
         * @brief �t���O�̃Z�b�g
         * @param flag
         * @param enable
         */
        void Set(Enum flag, bool enable);

        /**
         * @brief �t���O���Z�b�g����Ă���邩
         * @param flag
         * @return bool
         */
        bool Has(Enum flag) const;

        /**
         * @brief �t���O�̃N���A
         * @param flag
         */
        void Clear(Enum flag);

        /**
         * @brief �l�̎擾
         */
        EnumType ToValue() const;

        /* ���Z�q */

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
