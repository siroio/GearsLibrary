#pragma once
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <chrono>

class GameTimer final :
    public Glib::SingletonPtr<GameTimer>,
    public Glib::Internal::Interface::ISystem,
    public Glib::Internal::Function::UpdateOrderSet<Glib::Internal::Order::Update::TIME>
{
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<float>;
    friend Glib::WeakPtr<GameTimer> Glib::SingletonPtr<GameTimer>::Instance();
    GameTimer() = default;

public:
    void Update();

    /**
     * @brief ���O�̃t���[���ƍ��̃t���[���ԂŌo�߂�������
     * @return �b
     */
    static float DeltaTime();

    /**
     * @brief �^�C���X�P�[���̕ύX
     * @param timeScale
     */
    static void TimeScale(float timeScale = 1.0f);

    /**
     * @brief ���O�̃t���[���ƍ��̃t���[���ԂŌo�߂������� *�^�C���X�P�[���̉e�����󂯂Ȃ�
     * @return �b
     */
    static float UnscaledDeltaTime();

private:
    static inline Duration maxDeltaTime_{ 0.1f };
    static inline Duration deltaTime_{ 0.0f };
    static inline Duration unscaledDeltaTime_;
    static inline float timeScale_{ 1.0f };
    TimePoint prevTime_{ Clock::now() };
};
