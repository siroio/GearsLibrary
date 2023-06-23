#ifndef FUNC_ORDER_CONSTANT_H
#define FUNC_ORDER_CONSTANT_H

/**
 * @brief 各関数の実行順序を制御するための定数
 */
namespace Glib::Constant::Order
{
    namespace Update
    {
        /* UPDATE ORDER */

        constexpr int INPUT = -100;
        constexpr int TIME = INPUT + 1;
        constexpr int SCENE = TIME + 1;
        constexpr int GAMEOBJECT = SCENE + 1;
        constexpr int COMPONENT = 100;
    }
}

#endif // !FUNC_ORDER_CONSTANT_H
