#pragma once
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/KeyBoardKeys.h>

namespace Glib
{
    class InputSystem :
        public Glib::Internal::Interface::ISystem,
        public Glib::SingletonPtr<InputSystem>,
        public Glib::Internal::Function::UpdateOrderSet<Glib::Internal::Order::Update::INPUT>
    {
    private:
        InputSystem() = default;
        friend Glib::WeakPtr<InputSystem> SingletonPtr<InputSystem>::Instance();

    public:
        ~InputSystem();

        bool Initialize();
        void Update();

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

    private:
    };
}
