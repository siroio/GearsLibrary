#pragma once
/**
 * @brief 各関数の実行順序を制御するための定数
 */
namespace Glib::Internal::Order
{
    /* INITIALIZE ORDERS */
    namespace Initialzie
    {
        constexpr int DX12_INIT = -100;
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
