#ifndef GEAR_SINGLETON_H
#define GEAR_SINGLETON_H

#include <concepts>
#include <mutex>
#include <memory>

template<typename T>
concept SingletonConstructible = requires
{
    new T();
};

template<typename T>
concept SingletonDestructible = requires(T * t)
{
    delete t;
};

template<typename T>
concept SingletonInitializable = requires(T * t)
{
    {
        t->Init()
    } -> std::same_as<void>;
};

template<class T>
class Singleton
{
protected:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const T&) = delete;
    Singleton operator = (const T&) = delete;

public:
    static T& Instance()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_instance)
        {
            std::call_once(init_flag, []()
            {
                _instance.reset(new T());
                if constexpr (SingletonInitializable<T>)
                {
                    _instance->Init();
                }
            });
        }

        return *_instance;
    }

    static void Destroy() requires SingletonDestructible<T>
    {
        std::lock_guard<std::mutex> lock(_mutex);

        std::call_once(destroy_flag, []()
        {
            static std::unique_ptr<T> instance(std::move(_instance));
            _instance.reset(nullptr);
            if (instance)
            {
                delete instance.release();
            }
        });
    }

protected:
    static std::unique_ptr<T> _instance;
    static std::once_flag init_flag, destroy_flag;
    static std::mutex _mutex;
};

template<class T>
std::unique_ptr<T> Singleton<T>::_instance;

template<class T>
std::once_flag Singleton<T>::init_flag;

template<class T>
std::once_flag Singleton<T>::destroy_flag;

template<class T>
std::mutex Singleton<T>::_mutex;

#endif // !GEAR_SINGLETON_H