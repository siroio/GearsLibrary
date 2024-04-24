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
        MsgValue(unsigned int msgID, const T& msg);
        unsigned int GetMsgID() const override;
        const std::type_info& GetValueType() const override;
        const T& Msg() const;

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
        return typeid(T);
    }

    template<class T>
    inline const T& MsgValue<T>::Msg() const
    {
        return value_;
    }
}
