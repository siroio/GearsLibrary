#pragma once
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
        constexpr int SKYBOX = -100;
        constexpr int OBJECT = SKYBOX + 1;
        constexpr int EFFECT = OBJECT + 1;
        constexpr int PHYSIC = EFFECT + 1;
        constexpr int CAMERA = PHYSIC + 1;
        constexpr int UI = CAMERA + 1;
    }

    /* FINALIZE ORDERS */
    namespace Finalize
    {
        constexpr int GAMEOBJECT = -100;
    }
}
