#ifndef GEARS_IORDERFUNC_H
#define GEARS_IORDERFUNC_H

namespace GLib::Internal::Interface
{
    template<class ReturnType, class... Args>
    class IOrderFunc
    {
    protected:
        virtual ~IOrderFunc() = default;
    public:
        virtual ReturnType Call(const Args&... args) = 0;
        virtual bool IsDelete() = 0;
        virtual int Order() = 0;
    };
}

#endif // !GEARS_IORDERFUNC_H
