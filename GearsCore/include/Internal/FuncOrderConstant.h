#ifndef FUNC_ORDER_CONSTANT_H
#define FUNC_ORDER_CONSTANT_H

/**
 * @brief �e�֐��̎��s�����𐧌䂷�邽�߂̒萔
 */
namespace GLib::Constant::Order
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
