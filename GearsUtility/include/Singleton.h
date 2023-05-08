#ifndef GEAR_SINGLETON_H
#define GEAR_SINGLETON_H

#include <memory>
#include <mutex>

template<typename T>
class Singleton
{
private:
    inline static std::unique_ptr<T> instance;
    inline static std::mutex singleton_mutex;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

public:
    static T& Instance()
    {
        std::scoped_lock lock(singleton_mutex);
        if (!instance)
        {
            instance = std::make_unique<T>();
        }
        return *instance;
    }

    static void Release()
    {
        std::scoped_lock lock(singleton_mutex);
        if (instance)
        {
            instance.reset();
        }
    }
};

#endif // !GEAR_SINGLETON_H