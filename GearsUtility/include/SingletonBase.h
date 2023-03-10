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
        // デフォルトコンストラクタがないとSingletonにできない
        static_assert(std::is_default_constructible<T>::value,
            "Singleton must be default constructible");

        if (!instance)
        {
            // インスタンスの生成
            instance.reset(new(std::nothrow) T);

            // Initialize関数がある場合実行
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
        // デフォルトコンストラクタがないとSingletonにできない
        static_assert(std::is_default_constructible<T>::value,
            "Singleton must be default constructible");

        if (!instance)
        {
            // インスタンスの生成
            instance.reset(new(std::nothrow) T(std::forward<Args>(args)...));

            // Initialize関数がある場合引数を渡して実行
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