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
        * @brief �L�[�������Ă��邩
        * @param key �L�[�̎��
        * @return �����Ă��� true
        * @return �����Ă��� false
       */
        static bool GetKey(KeyCode key);

        /**
         * @brief �L�[����������
         * @param key �L�[�̎��
         * @return ������ true
        */
        static bool GetKeyDown(KeyCode key);

        /**
         * @brief �L�[�𗣂�����
         * @param key �L�[�̎��
         * @return ������ true
        */
        static bool GetKeyUp(KeyCode key);

    private:
    };
}
