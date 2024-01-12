#pragma once
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/KeyBoardKeys.h>
#include <Internal/GamePadKeys.h>
#include <Internal/MouseButton.h>

struct Vector2;

namespace Glib
{
    class InputSystem :
        public Internal::Interface::ISystem,
        public SingletonPtr<InputSystem>,
        public Internal::Function::UpdateOrderSet<Internal::Order::Update::INPUT>
    {
    private:
        InputSystem() = default;
        friend WeakPtr<InputSystem> SingletonPtr<InputSystem>::Instance();

    public:
        /**
         * @brief 入力デバイスの初期化
         */
        bool Initialize();

        /**
         * @brief 入力を取得
         */
        void Update();

        /**
         * @brief 入力の追加
         * @param name 入力名
         * @param button 入力ボタン
         */
        static void AddInput(const std::string& name, MouseButton button);

        /**
         * @brief 入力の追加
         * @param name 入力名
         * @param button 入力ボタン
         */
        static void AddInput(const std::string& name, KeyCode button);

        /**
         * @brief 入力の追加
         * @param name 入力名
         * @param button 入力ボタン
         */
        static void AddInput(const std::string& name, GPADKey button);

        /**
         * @brief 入力状態の取得
         * @param name 入力名
         */
        static bool GetInput(const std::string& name);

        /**
         * @brief 入力状態の取得
         * @param name 入力名
         */
        static bool GetInputDown(const std::string& name);

        /**
         * @brief 入力状態の取得
         * @param name 入力名
         */
        static bool GetInputUp(const std::string& name);

        /**
        * @brief キーを押しているか
        * @param key キーの種類
        * @return 押している true
        * @return 離している false
       */
        static bool GetKey(KeyCode key);

        /**
         * @brief キーを押したか
         * @param key キーの種類
         * @return 押した true
        */
        static bool GetKeyDown(KeyCode key);

        /**
         * @brief キーを離したか
         * @param key キーの種類
         * @return 離した true
        */
        static bool GetKeyUp(KeyCode key);

        /**
         * @brief ボタンを押しているか
         * @param button ボタンの種類
         * @param pad パッドのポート (0 ~ 3)
        */
        static bool GetButton(GPADKey button, unsigned int pad = 0);

        /**
         * @brief ボタンを押したか
         * @param button ボタンの種類
         * @param pad パッドのポート (0 ~ 3)
        */
        static bool GetButtonDown(GPADKey button, unsigned int pad = 0);

        /**
         * @brief ボタンを離したか
         * @param button ボタンの種類
         * @param pad パッドのポート (0 ~ 3)
        */
        static bool GetButtonUp(GPADKey button, unsigned int pad = 0);

        /**
         * @brief 左スティックの傾き具合を取得
         * @param pad パッドのポート (0 ~ 3)
         * @param deadZone デッドゾーン
        */
        static Vector2 GetLeftStick(unsigned int pad = 0, float deadZone = 0.2f);

        /**
         * @brief 右スティックの傾き具合を取得
         * @param pad パッドのポート (0 ~ 3)
         * @param deadZone デッドゾーン
        */
        static Vector2 GetRightStick(unsigned int pad = 0, float deadZone = 0.2f);

        /**
         * @brief 左トリガーの押され具合を取得する（0.0～1.0）
         * @param pad パッドのポート (0 ~ 3)
         * @param deadZone デッドゾーン
        */
        static float GetLeftTrigger(unsigned int pad = 0, float deadZone = 0.2f);

        /**
         * @brief 右トリガーの押され具合を取得する（0.0～1.0）
         * @param pad パッドのポート (0 ~ 3)
         * @param deadZone デッドゾーン
        */
        static float GetRightTrigger(unsigned int pad = 0, float deadZone = 0.2f);
    };
}
