#pragma once
/**
 * @brief 関数の実行順生成
 */
#define FUNC_ORDER(FuncName)                                            \
                                                                        \
namespace Glib::Internal                                                \
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
        class FuncName##OrderSet : public Interface::I##FuncName##Order \
        {                                                               \
        public:                                                         \
            virtual int FuncName##Order() override                      \
            {                                                           \
                return order;                                           \
            }                                                           \
        };                                                              \
    }                                                                   \
}

