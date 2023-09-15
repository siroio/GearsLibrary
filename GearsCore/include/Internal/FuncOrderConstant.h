#pragma once
/**
 * @brief �e�֐��̎��s�����𐧌䂷�邽�߂̒萔
 */
namespace Glib::Internal::Order
{
    /* ���������Ԉꗗ */
    namespace Initialzie
    {
        constexpr int DX12_INIT = -100;
    }

    /* �X�V���Ԉꗗ */
    namespace Update
    {
        constexpr int INPUT = -100;
        constexpr int TIME = INPUT + 1;
        constexpr int SCENE = TIME + 1;
        constexpr int GAMEOBJECT = SCENE + 1;
        constexpr int COMPONENT = 100;
    }

    /* �`��O���Ԉꗗ */
    namespace BeginDraw
    {
        constexpr int DX12_BEGINDRAW = -100;
    }

    /* �`�揇�Ԉꗗ */
    namespace Draw
    {
        constexpr int SKYBOX = -100;
        constexpr int OBJECT = SKYBOX + 1;
        constexpr int EFFECT = OBJECT + 1;
        constexpr int PHYSIC = EFFECT + 1;
        constexpr int CAMERA = PHYSIC + 1;
        constexpr int UI = CAMERA + 1;
    }

    /* �I�����Ԉꗗ */
    namespace Finalize
    {
        constexpr int GAMEOBJECT = -100;
    }
}
