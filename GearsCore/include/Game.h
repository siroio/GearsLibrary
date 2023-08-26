#pragma once

namespace Glib
{
    /**
 * @brief ���C���Q�[���N���X
 */
    class Game
    {
    protected:
        Game() = default;
        virtual ~Game() = default;

        /**
         * @brief �J�n����
         */
        virtual void Start() = 0;

        /**
         * @brief �I������
         */
        virtual void End() = 0;

    public:
        /**
         * @brief ���s
         */
        int Run();

        /**
         * @brief �I��
         */
        inline static void Exit()
        {
            isExit = true;
        }

    private:

        /**
         * @brief �V�X�e���̓o�^
        */
        void RegisterSystem();

        /**
         * @brief ������
        */
        bool Initialize();

        /**
         * @brief �X�V
        */
        void Update();

        /**
         * @brief �`��
        */
        void Draw();

        /**
         * @brief �I������
        */
        void Finalize();

    private:
        inline static bool isExit{ false };
    };
}
