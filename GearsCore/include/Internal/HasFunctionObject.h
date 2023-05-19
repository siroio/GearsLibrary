#ifndef GEARS_HAS_FUNCTION_OBJECT_H
#define GEARS_HAS_FUNCTION_OBJECT_H

#include <Internal/HasFunction.h>
#include <Internal/IOrderFunc.h>
#include <WeakPtr.h>

#define HasFuncObject(FuncName)                                                                                         \
                                                                                                                        \
HasFunction(##FuncName##)                                                                                               \
                                                                                                                        \
namespace GLib::Internal::Function                                                                                      \
{                                                                                                                       \
                                                                                                                        \
    template<typename T, typename ReturnType, typename... Args>                                                         \
    class Has##FuncName##Object : public Interface::IOrderFunc<ReturnType, Args...>                                     \
    {                                                                                                                   \
    public:                                                                                                             \
        Has##FuncName##Object(const Utility::WeakPtr<T>&ptr) : instance{ ptr }                                          \
        {}                                                                                                              \
                                                                                                                        \
        virtual ReturnType Call(const Args&... args) override                                                           \
        {                                                                                                               \
            return Run(args...);                                                                                        \
        }                                                                                                               \
                                                                                                                        \
        virtual bool IsDelete() override                                                                                \
        {                                                                                                               \
            return instance.expired();                                                                                  \
        }                                                                                                               \
                                                                                                                        \
        virtual int Order() override                                                                                    \
        {                                                                                                               \
            return FuncOrder(instance.get().get());                                                                     \
        }                                                                                                               \
                                                                                                                        \
    private:                                                                                                            \
                                                                                                                        \
        template<typename U = T> requires Has##FuncName##Func<U, ReturnType, Args...>                                   \
        ReturnType Run(const Args&... args)                                                                             \
        {                                                                                                               \
            return instance->##FuncName##(args...);                                                                     \
        }                                                                                                               \
                                                                                                                        \
        template<typename U = T> requires !Has##FuncName##Func<U, ReturnType, Args...>                                  \
        ReturnType Run(...)                                                                                             \
        {                                                                                                               \
            return ReturnType{};                                                                                        \
        }                                                                                                               \
                                                                                                                        \
        template<typename U = T> requires !Has##FuncName##Func<U, ReturnType, Args...> && !std::is_void_v<ReturnType>   \
        ReturnType Run(...)                                                                                             \
        {}                                                                                                              \
                                                                                                                        \
        template<typename U = T> requires std::derived_from<U, Interface::I##FuncName##Order>                           \
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
        Utility::WeakPtr<T> instance{ nullptr };                                                                        \
    };                                                                                                                  \
}                                                                                                                       \

#endif // !HAS_FUNCTION_OBJECT_H
