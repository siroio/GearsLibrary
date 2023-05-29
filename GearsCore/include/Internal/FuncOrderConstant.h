#ifndef FUNC_ORDER_CONSTANT_H
#define FUNC_ORDER_CONSTANT_H

namespace GLib::Constant::Order
{
    namespace Update
    {
        /* UPDATE ORDER */
        constexpr int COMPONENT = 100;
    }

    namespace Finalize
    {
        /* FINALIZE ORDER */
        constexpr int COMPONENT = -100;
    }
}

#endif // !FUNC_ORDER_CONSTANT_H
