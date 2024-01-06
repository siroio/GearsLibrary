#pragma once

struct Vector2;

namespace Glib
{
    enum class GPADKey;
}

namespace Glib::Internal::Interface
{
    // ゲームパッド用共通インターフェース
    class IGamePad
    {
    public:
        IGamePad() = default;
        virtual ~IGamePad() = default;

    public:

        /**
         * @brief キーが押されているか
         * @param controller コントローラーインデックス
         * @param button ボタン
         */
        virtual bool GetButton(unsigned int controller, GPADKey button) = 0;

        /**
         * @brief キーが押されたか
         * @param controller コントローラーインデックス
         * @param button ボタン
         */
        virtual bool GetButtonDown(unsigned int controller, GPADKey button) = 0;

        /**
         * @brief キーが押されたか
         * @param controller コントローラーインデックス
         * @param button ボタン
         */
        virtual bool GetButtonUp(unsigned int controller, GPADKey button) = 0;

        /**
         * @brief 右スティックの方向を取得
         * @param controller コントローラーインデックス
         * @param button ボタン
         * @return 方向
         */
        virtual Vector2 GetRightStick(unsigned int controller, float deadZone) = 0;

        /**
         * @brief 左スティックの方向を取得
         * @param controller コントローラーインデックス
         * @param deadZone デッドゾーン
         * @return 方向
         */
        virtual Vector2 GetLeftStick(unsigned int controller, float deadZone) = 0;

        /**
         * @brief 右のトリガーを取得
         * @param controller コントローラーインデックス
         * @param deadZone デッドゾーン
         */
        virtual float GetRightTrigger(unsigned int controller, float deadZone) = 0;

        /**
         * @brief 左のトリガーを取得
         * @param controller コントローラーインデックス
         * @param deadZone デッドゾーン
         */
        virtual float GetLeftTrigger(unsigned int controller, float deadZone) = 0;
    };
}
