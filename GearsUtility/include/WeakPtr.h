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
        WeakPtr(const std::shared_ptr<T>& ptr) : ptr_{ ptr }
        {}
        WeakPtr(const std::weak_ptr<T>& ptr) : ptr_{ ptr }
        {}
        WeakPtr(const WeakPtr<T>& ptr) : ptr_{ ptr.ptr_ }
        {}

        // ダウンキャスト
        template <class Base> requires std::derived_from<T, Base>
        inline WeakPtr(const std::shared_ptr<Base>& ptr) : ptr_{ std::dynamic_pointer_cast<T>(ptr) }
        {}

        template <class Base> requires std::derived_from<T, Base>
        inline WeakPtr(const std::weak_ptr<Base>& ptr) : ptr_{ std::dynamic_pointer_cast<T>(ptr.lock()) }
        {}

        template <class Base> requires std::derived_from<T, Base>
        inline WeakPtr(const WeakPtr<Base>& ptr) : ptr_{ std::dynamic_pointer_cast<T>(ptr.get()) }
        {}

        // アップキャスト
        template <class Drived> requires std::derived_from<Drived, T>
        inline WeakPtr(const std::shared_ptr<Drived>& ptr) : ptr_{ ptr }
        {}
        template <class Drived> requires std::derived_from<Drived, T>
        inline WeakPtr(const std::weak_ptr<Drived>& ptr) : ptr_{ ptr.lock() }
        {}
        template <class Drived> requires std::derived_from<Drived, T>
        inline WeakPtr(const WeakPtr<Drived>& ptr) : ptr_{ ptr.get() }
        {}

        std::shared_ptr<T> get() const
        {
            auto ptr = ptr_.lock();
            if (!ptr) throw std::bad_weak_ptr();
            return ptr;
        }

        bool expired() const
        {
            return ptr_.expired();
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
            return ptr_.lock() == other.get();
        }

        bool operator != (const WeakPtr<T>& other) const
        {
            return ptr_.lock() != other.get();
        }

        template<class U>
        bool operator == (const WeakPtr<U> other) const
        {
            return ptr_.lock() == other.get();
        }

        template<class U>
        bool operator != (const WeakPtr<U> other) const
        {
            return ptr_.lock() != other.get();
        }

    private:
        std::weak_ptr<T> ptr_;
    };
}

#endif // !GEARS_WEAKPTR_H
