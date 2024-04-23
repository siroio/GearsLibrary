#include "EventMsg.h"

unsigned int Glib::EventMsg::MsgID() const
{
    return msgValue_->GetMsgID();
}
