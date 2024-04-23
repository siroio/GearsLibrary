#pragma once
#include <typeinfo>

namespace Glib::Internal::Interface
{
    class IMsgValue
    {
    public:
        virtual ~IMsgValue() = default;
        virtual unsigned int GetMsgID() const = 0;
        virtual const std::type_info& GetValueType() const = 0;
    };
}
