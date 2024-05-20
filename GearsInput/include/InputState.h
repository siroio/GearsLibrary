#pragma once
#include <string>

namespace Glib
{
    enum class MouseButton : unsigned;
    enum class KeyCode : unsigned;
    enum class GPADKey : unsigned;

    enum class InputType
    {
        MOUSE,
        KEYBOARD,
        GAMEPAD,
    };

    /**
     * @brief 入力データ管理クラス
     */
    class InputState
    {
    public:
        InputState(MouseButton button);
        InputState(KeyCode button);
        InputState(GPADKey button);

        /**
         * @brief 入力情報
         */
        union
        {
            MouseButton mouse;
            KeyCode keyboard;
            GPADKey pad;
        };

        /**
         * @brief 入力種類
         */
        InputType type;
    };
}
