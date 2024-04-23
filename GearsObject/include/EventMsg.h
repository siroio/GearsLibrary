#pragma once
#include <Internal/MsgValue.h>
#include <memory>

namespace Glib
{
    class EventMsg
    {
    public:

        template<class T>
        EventMsg(unsigned int msgID, const T& value);

        /**
         * @brief メッセージIDの取得
         * @return
         */
        unsigned int MsgID() const;

        /**
         * @brief 内容を取得
         * @return
         */
        template<class T>
        const T& Msg() const;

    private:
        std::unique_ptr<Internal::Interface::IMsgValue> msgValue_{ nullptr };
    };

    template<class T>
    inline EventMsg::EventMsg(unsigned int msgID, const T& value) :
        msgValue_{ std::make_unique<Internal::MsgValue<T>>(msgID, value) }
    {}

    template<class T>
    inline const T& EventMsg::Msg() const
    {
        return static_cast<Internal::MsgValue<T>*>(msgValue_.get())->Msg();
    }
}
