#pragma once
#include <Internal/IMsgValue.h>

namespace Glib::Internal
{
    /**
     * @brief T型の値を管理するクラス
     */
    template<class T>
    class MsgValue : public Interface::IMsgValue
    {
    public:
        using value = T;
        MsgValue(unsigned int msgID, const T& msg);
        unsigned int GetMsgID() const override;
        const std::type_info& GetValueType() const override;
        const value& Msg() const;

    private:
        unsigned int msgID_{};
        T value_;
    };

    template<class T>
    inline MsgValue<T>::MsgValue(unsigned int msgID, const T& value) :
        msgID_{ msgID }, value_{ value }
    {}

    template<class T>
    inline unsigned int MsgValue<T>::GetMsgID() const
    {
        return msgID_;
    }

    template<class T>
    inline const std::type_info& MsgValue<T>::GetValueType() const
    {
        return typeid(MsgValue<T>::value);
    }

    template<class T>
    inline const MsgValue<T>::value& MsgValue<T>::Msg() const
    {
        return value_;
    }
}
