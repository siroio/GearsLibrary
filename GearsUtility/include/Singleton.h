﻿#pragma once
#include <WeakPtr.h>
#include <mutex>

namespace Glib
{
    /**
     * @brief 通常のSingletonクラス
     */
    template<class T>
    class Singleton
    {
    protected:
        static inline std::mutex singleton_mutex;

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

    public:
        /**
         * @brief インスタンスの取得
         */
        static inline T& Instance()
        {
            std::lock_guard lock{ singleton_mutex };
            static T instance;
            return instance;
        }
    };

    /**
     * @brief ポインタ版Singletonクラス
     */
    template<class T>
    class SingletonPtr
    {
    protected:
        static inline std::shared_ptr<T> instance;
        static inline std::mutex singleton_mutex;

    protected:
        SingletonPtr() = default;
        virtual ~SingletonPtr() = default;
        SingletonPtr(const SingletonPtr&) = delete;
        SingletonPtr& operator=(const SingletonPtr&) = delete;

    public:
        /**
         * @brief インスタンスの取得
         */
        template<class... Args>
        static inline WeakPtr<T> Instance(const Args&... args)
        {
            std::lock_guard lock{ singleton_mutex };
            if (!instance)
            {
                instance = std::make_shared<T>(std::forward<Args>(args)...);
            }
            return WeakPtr<T>{ instance };
        }

        /**
         * @brief インスタンスの解放
         */
        static inline void Release()
        {
            std::lock_guard lock{ singleton_mutex };
            instance.reset();
        }
    };
};
