#ifndef GEAR_SINGLETON_H
#define GEAR_SINGLETON_H

#include <memory>
#include <mutex>

template<typename T>
concept HasInit = requires(T t)
{
    {
        t.Init()
    } -> std::same_as<void>;
};

template<typename T>
class Singleton
{
protected:
    static std::unique_ptr<T> instance;
    static std::mutex singleton_mutex;

    Singleton() = default;
    ~Singleton() = default;
    Singleton(const T&) = delete;
    Singleton operator = (const T&) = delete;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;

    static T& Instance()
    {
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if (!instance)
        {
            instance = std::make_unique<T>();
            if constexpr (HasInit<T>)
            {
                instance->Init();
            }
        }
        return *instance.get();
    }

    static void Release()
    {
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if (instance)
        {
            instance.reset();
        }
    }
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance;

template<typename T>
std::mutex Singleton<T>::singleton_mutex;

#endif // !GEAR_SINGLETON_H