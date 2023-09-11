#pragma once
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <chrono>

namespace Glib
{
    class GameTimer final :
        public SingletonPtr<GameTimer>,
        public Internal::Interface::ISystem,
        public Internal::Function::UpdateOrderSet<Internal::Order::Update::TIME>
    {
    private:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using Duration = std::chrono::duration<float>;
        friend WeakPtr<GameTimer> SingletonPtr<GameTimer>::Instance();
        GameTimer() = default;

    public:
        void Update();

        /**
         * @brief ���O�̃t���[���ƍ��̃t���[���ԂŌo�߂�������
         * @return �b
         */
        static float DeltaTime();

        /**
         * @brief �Œ�t���[�����[�g�̍X�V�����s����C���^�[�o��
         * @return �b
        */
        static float FixedDeltaTime();

        /**
         * @brief �^�C���X�e�b�v���擾
         * @param timeStep
        */
        static float FixedTimeStep();

        /**
         * @brief �^�C���X�e�b�v��ݒ�
         * @param timeStep
        */
        static void FixedTimeStep(float timeStep);

        /**
         * @brief �^�C���e�b�v�̏���l�擾
         * @param maximumAllowedTimestep
        */
        static float MaximumAllowedTimeStep();

        /**
         * @brief �^�C���e�b�v�̏���l�ݒ�
         * @param maximumAllowedTimeStep
        */
        static void MaximumAllowedTimeStep(float maximumAllowedTimeStep);

        /**
         * @brief �^�C���X�P�[���̕ύX
         * @param timeScale
         */
        static void TimeScale(float timeScale = 1.0f);

        /**
         * @brief ���O�̃t���[���ƍ��̃t���[���ԂŌo�߂�������
         * @brief �^�C���X�P�[���̉e�����󂯂Ȃ�
         * @return �b
         */
        static float UnscaledDeltaTime();

    private:
        static inline Duration maxDeltaTime_{ 0.1f };
        static inline Duration deltaTime_{ 0.0f };
        static inline Duration unscaledDeltaTime_{ 1.0f };
        static inline Duration maximumAllowedTimeStep_{ 0.3333333f };
        static inline float timeScale_{ 1.0f };
        static inline float fixedTimeStep_{ 0.02f };
        TimePoint prevTime_{ Clock::now() };
    };
}
