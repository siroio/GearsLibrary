#ifndef GEARS_SINGLETON_H
#define GEARS_SINGLETON_H

#include <memory>
#include <mutex>
#include <WeakPtr.h>

namespace GLib::Utility
{
    /**
     * @brief ポインタ版Singleton
     */
    template<class T>
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
        /**
         * @brief インスタンスの取得
         */
        static inline WeakPtr<T> Instance()
        {
            std::scoped_lock lock(singleton_mutex);
            if (!instance)
            {
                instance = std::shared_ptr<T>{ new T() };
            }
            return WeakPtr<T>{ instance };
        }

        /**
         * @brief インスタンスの解放
         */
        static inline void Release()
        {
            std::scoped_lock lock(singleton_mutex);
            if (instance)
            {
                instance.reset();
            }
        }
    };

    /**
     * @brief 通常のSingletonクラス
     */
    template<class T>
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
        /**
         * @brief インスタンスの取得
         */
        static inline T& Instance()
        {
            std::scoped_lock lock(singleton_mutex);
            static T instance;
            return instance;
        }
    };
};

#endif // !GEARS_SINGLETON_H
