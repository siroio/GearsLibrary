#ifndef GEARS_SINGLETON_H
#define GEARS_SINGLETON_H

#include <memory>
#include <mutex>
#include <WeakPtr.h>

namespace GLib::Utility
{
    template<typename T>
    class SingletonPtr
    {
    private:
        static inline std::shared_ptr<T> instance;
        static inline std::mutex singleton_mutex;

    protected:
        SingletonPtr() = default;
        virtual ~SingletonPtr() = default;

    public:
        SingletonPtr(const SingletonPtr&) = delete;
        SingletonPtr& operator=(const SingletonPtr&) = delete;
        SingletonPtr(SingletonPtr&&) = delete;
        SingletonPtr& operator=(SingletonPtr&&) = delete;

    public:
        static inline WeakPtr<T> Instance()
        {
            std::scoped_lock lock(singleton_mutex);
            if (!instance)
            {
                instance = std::make_shared<T>();
            }
            return WeakPtr<T>{ instance };
        }

        static inline void Release()
        {
            std::scoped_lock lock(singleton_mutex);
            if (instance)
            {
                instance.reset();
            }
        }
    };

    template<typename T>
    class Singleton
    {
    private:
        static inline std::mutex singleton_mutex;

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    public:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

    public:
        static inline T& Instance()
        {
            std::scoped_lock lock(singleton_mutex);
            static T instance;
            return instance;
        }
    };
};

#endif // !GEARS_SINGLETON_H