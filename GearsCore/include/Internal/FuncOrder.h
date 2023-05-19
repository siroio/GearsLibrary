#ifndef GEARS_FUNC_ORDER_H
#define GEARS_FUNC_ORDER_H

#define FUNC_ORDER(FuncName)                                             \
                                                                        \
namespace GLib::Internal                                                \
{                                                                       \
    namespace Interface                                                 \
    {                                                                   \
        class I##FuncName##Order                                        \
        {                                                               \
        protected:                                                      \
            I##FuncName##Order() = default;                             \
            virtual ~I##FuncName##Order() = default;                    \
        public:                                                         \
            virtual int FuncName##Order() = 0;                          \
        };                                                              \
    }                                                                   \
                                                                        \
    namespace Function                                                  \
    {                                                                   \
        template<int order>                                             \
        class FuncName##SetOrder : public Interface::I##FuncName##Order \
        {                                                               \
        public:                                                         \
            virtual int FuncName##Order() override                      \
            {                                                           \
                return order;                                           \
            }                                                           \
        };                                                              \
    }                                                                   \
}

#endif // !GEARS_FUNC_ORDER_H
