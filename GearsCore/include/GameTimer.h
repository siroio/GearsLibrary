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
    using clock = std::chrono::high_resolution_clock;
    using timePoint = clock::time_point;
    using duration = std::chrono::duration<float>;
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
    static inline duration maxDeltaTime_{ 0.1f };
    static inline duration deltaTime_{ 0.0f };
    static inline duration unscaledDeltaTime_;
    static inline float timeScale_{ 1.0f };
    timePoint prevTime_{ clock::now() };
};
