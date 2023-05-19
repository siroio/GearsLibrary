#ifndef GEARS_HAS_FUNCTION_H
#define GEARS_HAS_FUNCTION_H

#include <concepts>

#define HAS_FUNCTION(FuncName)                                          \
namespace GLib::Internal                                                \
{                                                                       \
    template<class T, class ReturnType = void, class... Args>  \
    concept Has##FuncName##Func = requires (T* t, Args... args)         \
    {                                                                   \
        {                                                               \
            t->##FuncName##(args...)                                    \
        } -> std::same_as<ReturnType>;                                  \
    };                                                                  \
}

#endif // !GEARS_HAS_FUNCTION_H
