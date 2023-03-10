#pragma once

#include <type_traits>
#include <memory>

template<class T>
class SingletonBase
{
private:
    static std::unique_ptr<T> instance{ nullptr };

public:
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase operator = (const SingletonBase&) = delete;
    SingletonBase& operator = (const SingletonBase&) = delete;

    static T& Instance()
    {
        // �f�t�H���g�R���X�g���N�^���Ȃ���Singleton�ɂł��Ȃ�
        static_assert(std::is_default_constructible<T>::value,
            "Singleton must be default constructible");

        if (!instance)
        {
            // �C���X�^���X�̐���
            instance.reset(new(std::nothrow) T);

            // Initialize�֐�������ꍇ���s
            if constexpr (std::is_invocable_v<decltype(&T::Initialize), T>)
            {
                instance->Initialize();
            }
        }

        return *instance;
    }

    template<typename... Args>
    static T& Instance(Args&&... args)
    {
        // �f�t�H���g�R���X�g���N�^���Ȃ���Singleton�ɂł��Ȃ�
        static_assert(std::is_default_constructible<T>::value,
            "Singleton must be default constructible");

        if (!instance)
        {
            // �C���X�^���X�̐���
            instance.reset(new(std::nothrow) T(std::forward<Args>(args)...));

            // Initialize�֐�������ꍇ������n���Ď��s
            if constexpr (std::is_invocable_v<decltype(&T::Initialize), T, Args...>)
            {
                std::invoke(&T::Initialize, *instance, std::forward<Args>(args)...);
            }
        }

        return *instance;
    }

protected:
    SingletonBase() = default;
    ~SingletonBase() = default;
};

template<class T>
std::unique_ptr<T> SingletonBase<T>::instance{ nullptr };