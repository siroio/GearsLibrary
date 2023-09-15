#pragma once
/**
 * @brief 各関数の実行順序を制御するための定数
 */
namespace Glib::Internal::Order
{
    /* 初期化順番一覧 */
    namespace Initialzie
    {
        constexpr int DX12_INIT = -100;
    }

    /* 更新順番一覧 */
    namespace Update
    {
        constexpr int INPUT = -100;
        constexpr int TIME = INPUT + 1;
        constexpr int SCENE = TIME + 1;
        constexpr int GAMEOBJECT = SCENE + 1;
        constexpr int COMPONENT = 100;
    }

    /* 描画前順番一覧 */
    namespace BeginDraw
    {
        constexpr int DX12_BEGINDRAW = -100;
    }

    /* 描画順番一覧 */
    namespace Draw
    {
        constexpr int SKYBOX = -100;
        constexpr int OBJECT = SKYBOX + 1;
        constexpr int EFFECT = OBJECT + 1;
        constexpr int PHYSIC = EFFECT + 1;
        constexpr int CAMERA = PHYSIC + 1;
        constexpr int UI = CAMERA + 1;
    }

    /* 終了順番一覧 */
    namespace Finalize
    {
        constexpr int GAMEOBJECT = -100;
    }
}
