#ifndef FUNC_ORDER_CONSTANT_H
#define FUNC_ORDER_CONSTANT_H

/**
 * @brief äeä÷êîÇÃé¿çsèáèòÇêßå‰Ç∑ÇÈÇΩÇﬂÇÃíËêî
 */
namespace Glib::Internal::Order
{
    /* INITIALIZE ORDERS */
    namespace Initialzie
    {
        constexpr int DX12_INIT = -100;
    }

    /* FIXEDUPDATE ORDERS */
    namespace FixedUpdate
    {
        constexpr int TIME = -100;
    }

    /* UPDATE ORDERS */
    namespace Update
    {
        constexpr int INPUT = -100;
        constexpr int TIME = INPUT + 1;
        constexpr int SCENE = TIME + 1;
        constexpr int GAMEOBJECT = SCENE + 1;
        constexpr int COMPONENT = 100;
    }

    /* BEGINDRAW ORDERS */
    namespace BeginDraw
    {
        constexpr int DX12_BEGINDRAW = -100;
    }

    /* DRAW ORDERS */
    namespace Draw
    {

    }

    /* FINALIZE ORDERS */
    namespace Finalize
    {

    }
}

#endif // !FUNC_ORDER_CONSTANT_H
