#pragma once
#include <WeakPtr.h>
#include <mutex>

namespace Glib
{
    /**
     * @brief �|�C���^��Singleton
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
        SingletonPtr(SingletonPtr&&) = delete;
        SingletonPtr& operator=(SingletonPtr&&) = delete;

    public:
        /**
         * @brief �C���X�^���X�̎擾
         */
        template<class... Args>
        static inline WeakPtr<T> Instance(const Args&... args)
        {
            std::lock_guard lock{ singleton_mutex };
            if (!instance)
            {
                instance = std::shared_ptr<T>{ new T{ args&&... } };
            }
            return WeakPtr<T>{ instance };
        }

        /**
         * @brief �C���X�^���X�̉��
         */
        static inline void Release()
        {
            std::lock_guard lock{ singleton_mutex };
            if (instance)
            {
                instance.reset();
            }
        }
    };

    /**
     * @brief �ʏ��Singleton�N���X
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
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

    public:
        /**
         * @brief �C���X�^���X�̎擾
         */
        static inline T& Instance()
        {
            std::lock_guard lock{ singleton_mutex };
            static T instance;
            return instance;
        }
    };
};
