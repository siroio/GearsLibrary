#ifndef GEARS_WEAKPTR_H
#define GEARS_WEAKPTR_H

#include <memory>
#include <concepts>

namespace GLib::Utility
{
    template<class T>
    class WeakPtr
    {
    public:

        WeakPtr() = default;
        WeakPtr(const std::shared_ptr<T>& ptr) : m_weak_ptr{ ptr }
        {}
        WeakPtr(const std::weak_ptr<T>& ptr) : m_weak_ptr{ ptr }
        {}
        WeakPtr(const WeakPtr<T>& ptr) : m_weak_ptr{ ptr.m_weak_ptr }
        {}

        // ダウンキャスト
        template<class U> requires std::derived_from<U, T>
        WeakPtr(const std::shared_ptr<U>& ptr) : m_weak_ptr{ std::dynamic_pointer_cast<T, U>(ptr) }
        {}
        template<class U> requires std::derived_from<U, T>
        WeakPtr(const std::weak_ptr<U>& ptr) : m_weak_ptr{ std::dynamic_pointer_cast<T, U>(ptr.lock()) }
        {}
        template<class U> requires std::derived_from<U, T>
        WeakPtr(const WeakPtr<U>& ptr) : m_weak_ptr{ std::dynamic_pointer_cast<T, U>(ptr.get()) }
        {}

        // アップキャスト
        template<class U> requires std::derived_from<T, U>
        WeakPtr(const std::shared_ptr<U>& ptr) : m_weak_ptr{ std::static_pointer_cast<T>(ptr) }
        {}
        template<class U> requires std::derived_from<T, U>
        WeakPtr(const std::weak_ptr<U>& ptr) : m_weak_ptr{ std::static_pointer_cast<T>(ptr.lock()) }
        {}
        template<class U> requires std::derived_from<T, U>
        WeakPtr(const WeakPtr<U>& ptr) : m_weak_ptr{ std::static_pointer_cast<T>(ptr.get()) }
        {}

        std::shared_ptr<T> get() const
        {
            auto ptr = m_weak_ptr.lock();
            if (!ptr) throw std::bad_weak_ptr();
            return ptr;
        }

        bool expired()
        {
            return m_weak_ptr.expired();
        }

        std::uintptr_t getId()
        {
            return reinterpret_cast<std::uintptr_t>(get().get());
        }

        std::shared_ptr<T> operator -> ()
        {
            return get();
        }

        T& operator * ()
        {
            return *get();
        }

        bool operator == (const WeakPtr<T>& other) const
        {
            return m_weak_ptr.lock() == other.get();
        }

        bool operator != (const WeakPtr<T>& other) const
        {
            return m_weak_ptr.lock() != other.get();
        }

        template<class U>
        bool operator == (const WeakPtr<U> other) const
        {
            return m_weak_ptr.lock() == other.get();
        }

        template<class U>
        bool operator != (const WeakPtr<U> other) const
        {
            return m_weak_ptr.lock() != other.get();
        }

    private:
        std::weak_ptr<T> m_weak_ptr;
    };
}

#endif // !GEARS_WEAKPTR_H
