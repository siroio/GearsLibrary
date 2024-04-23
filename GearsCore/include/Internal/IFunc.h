#pragma once

namespace Glib::Internal::Interface
{
    /**
     * @brief 関数管理用インターフェース
     */
    template<class ReturnType, class... Args>
    class IFunc
    {
    protected:
        virtual ~IFunc() = default;

    public:
        virtual ReturnType Call(const Args&... args) = 0;
        virtual bool IsDelete() = 0;
        virtual int Order() = 0;
    };
}
