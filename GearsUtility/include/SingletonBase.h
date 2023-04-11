#pragma once

#include <type_traits>
#include <memory>

template<typename T>
class Singleton
{
private:
    static std::unique_ptr<T> instance_;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    template<typename... Args>
    static T& Instance(Args&&... args)
    {
        static_assert(std::is_constructible_v<T, Args...>,
            "Singleton class must be constructible with provided arguments.");

        if (!instance_)
        {
            instance_ = std::make_unique<T>(std::forward<Args>(args)...);

            if constexpr (std::is_invocable_v<decltype(&T::Initialize), T, Args...>)
            {
                std::invoke(&T::Initialize, *instance_, std::forward<Args>(args)...);
            }
        }

        return *instance_;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance_{ nullptr };