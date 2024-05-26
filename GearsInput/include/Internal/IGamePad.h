#pragma once

struct Vector2;

namespace Glib
{
    enum class GPADKey : unsigned;
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
         * @brief 初期化
         * @param controllerIndex コントローラーの番号
         */
        virtual bool Initialize(DWORD controllerIndex) = 0;

        /**
         * @brief 更新
         */
        virtual void Update() = 0;

        /**
         * @brief キーが押されているか
         * @param button ボタン
         */
        virtual bool GetButton(GPADKey button) = 0;

        /**
         * @brief キーが押されたか
         * @param button ボタン
         */
        virtual bool GetButtonDown(GPADKey button) = 0;

        /**
         * @brief キーが押されたか
         * @param button ボタン
         */
        virtual bool GetButtonUp(GPADKey button) = 0;

        /**
         * @brief 左スティックの方向を取得
         * @param deadZone デッドゾーン
         * @return 方向
         */
        virtual Vector2 GetLeftStick(float deadZone) = 0;

        /**
         * @brief 右スティックの方向を取得
         * @param button ボタン
         * @return 方向
         */
        virtual Vector2 GetRightStick(float deadZone) = 0;

        /**
         * @brief 左のトリガーを取得
         * @param deadZone デッドゾーン
         */
        virtual float GetLeftTrigger(float deadZone) = 0;


        /**
         * @brief 右のトリガーを取得
         * @param deadZone デッドゾーン
         */
        virtual float GetRightTrigger(float deadZone) = 0;

        /**
         * @brief ゲームパッドを振動させる
         * @param left 左
         * @param right 右
         * @param time 時間
         */
        virtual void VibratePad(float left, float right, float time) = 0;
    };
}
