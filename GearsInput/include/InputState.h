#pragma once
#include <string>
#include <variant>

namespace Glib
{
    enum class MouseButton : unsigned;
    enum class KeyCode : unsigned;
    enum class GPADKey : unsigned;

    /**
     * @brief 入力の種類
     */
    struct InputType
    {
        enum
        {
            MOUSE = 0x0,
            KEYBOARD = 0x1,
            GAMEPAD = 0x2,
        };
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
        std::variant<MouseButton, KeyCode, GPADKey> input;
    };
}
