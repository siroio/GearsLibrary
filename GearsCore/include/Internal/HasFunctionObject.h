#pragma once
#include <concepts>
#include <WeakPtr.h>
#include <Internal/IFunc.h>

/**
 * @brief 特定の関数を持ったオブジェクトの定義作成
 */
#define HAS_FUNC_OBJECT(FuncName)                                                                                       \
                                                                                                                        \
namespace Glib::Internal                                                                                                \
{                                                                                                                       \
    template<class T, class ReturnType, class... Args>                                                                  \
    concept Has##FuncName##Func = requires (T* t, Args... args)                                                         \
    {                                                                                                                   \
        {                                                                                                               \
            t->##FuncName##(args...)                                                                                    \
        } -> std::same_as<ReturnType>;                                                                                  \
    };                                                                                                                  \
}                                                                                                                       \
                                                                                                                        \
namespace Glib::Internal::Function                                                                                      \
{                                                                                                                       \
                                                                                                                        \
    template<class T, class ReturnType, class... Args>                                                                  \
    class Has##FuncName##Object : public Interface::IFunc<ReturnType, Args...>                                          \
    {                                                                                                                   \
    public:                                                                                                             \
        Has##FuncName##Object(const WeakPtr<T>&ptr) : instance_{ ptr }                                                  \
        {}                                                                                                              \
                                                                                                                        \
        virtual ReturnType Call(const Args&... args) override                                                           \
        {                                                                                                               \
            return Run(args...);                                                                                        \
        }                                                                                                               \
                                                                                                                        \
        virtual bool IsDelete() override                                                                                \
        {                                                                                                               \
            return instance_.expired();                                                                                 \
        }                                                                                                               \
                                                                                                                        \
        virtual int Order() override                                                                                    \
        {                                                                                                               \
            return FuncOrder(instance_.get().get());                                                                    \
        }                                                                                                               \
                                                                                                                        \
    private:                                                                                                            \
        template<class U = T> requires Has##FuncName##Func<U, ReturnType, Args...>                                      \
        ReturnType Run(const Args&... args)                                                                             \
        {                                                                                                               \
            return instance_->##FuncName##(args...);                                                                    \
        }                                                                                                               \
                                                                                                                        \
        template<class U = T> requires !Has##FuncName##Func<U, ReturnType, Args...>                                     \
        ReturnType Run(...)                                                                                             \
        {                                                                                                               \
            return ReturnType{};                                                                                        \
        }                                                                                                               \
                                                                                                                        \
        template<class U = T> requires !Has##FuncName##Func<U, ReturnType, Args...> && !std::is_void_v<ReturnType>      \
        ReturnType Run(...)                                                                                             \
        {}                                                                                                              \
                                                                                                                        \
        template<class U = T> requires std::derived_from<U, Interface::I##FuncName##Order>                              \
        int FuncOrder(U* self)                                                                                          \
        {                                                                                                               \
            return self->##FuncName##Order();                                                                           \
        }                                                                                                               \
        int FuncOrder(...)                                                                                              \
        {                                                                                                               \
            return 0;                                                                                                   \
        }                                                                                                               \
                                                                                                                        \
    private:                                                                                                            \
        WeakPtr<T> instance_{ nullptr };                                                                                \
    };                                                                                                                  \
}                                                                                                                       \
