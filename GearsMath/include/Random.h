#pragma once
#include <atomic>
#include <memory>
#include <Singleton.h>
#include <Internal/ISystem.h>
#include <Mathf.h>

namespace Glib
{
    /**
     * @brief Permuted Congruential Generator �^������������
     */
    class Pcg32Fast
    {
    public:
        explicit Pcg32Fast(uint64_t seed);

        /**
         * @brief ���݂̃V�[�h���擾
         */
        uint64_t Seed() const;

        /**
         * @brief �V�[�h��ݒ�
         */
        void Seed(uint64_t seed = 0x853c49e6748fea9bull);

        /**
         * @brief ����
         */
        uint32_t Gen();

    public:
        static constexpr uint64_t MULTIPLIER = 6364136223846793005u;
        static constexpr uint64_t INCREMENT = 1442695040888963407u;
        static constexpr float DIVIDE = static_cast<float>(1.0f / 4294967295.0);

    private:
        std::atomic_uint64_t seed;
        std::atomic_uint64_t state;
    };

    /**
     * @brief �������� ���b�p�[�N���X
     */
    class Random :
        public Internal::Interface::ISystem,
        public SingletonPtr<Random>
    {
        Random() = default;
        friend WeakPtr<Random> SingletonPtr<Random>::Instance();

    public:

        /**
         * @brief ������
         */
        bool Initialize();

        /**
         * @brief �V�[�h�l�̐ݒ�
         * @param seed
         */
        static void Seed(uint64_t seed);

        /**
         * @brief �����𐶐�
         * @return
         */
        static uint32_t Next();

        /**
         * @brief �����_�̗����𐶐�
         * @return
         */
        static float Nextf();

        /**
         * @brief min ~ max�͈͂ŗ����𐶐�
         * @param min
         * @param max
         * @return
         */
        static int Range(int min, int max);

        /**
         * @brief min ~ max�͈͂ŏ����_�̗����𐶐�
         * @param min
         * @param max
         * @return
         */
        static float Range(float min, float max);

    private:
        static inline std::unique_ptr<Pcg32Fast> generator;
    };
}
