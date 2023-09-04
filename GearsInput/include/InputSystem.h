#pragma once
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/KeyBoardKeys.h>

namespace Glib
{
    class InputSystem :
        public Internal::Interface::ISystem,
        public SingletonPtr<InputSystem>,
        public Internal::Function::UpdateOrderSet<Internal::Order::Update::INPUT>
    {
    private:
        InputSystem() = default;
        friend WeakPtr<InputSystem> Glib::SingletonPtr<InputSystem>::Instance();

    public:
        /**
         * @brief ���̓f�o�C�X�̏�����
         */
        bool Initialize();

        /**
         * @brief ���͂��擾
         */
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
    };
}
