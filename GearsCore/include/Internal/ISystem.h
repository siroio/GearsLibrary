#ifndef GEARS_ISYSTEM_H
#define GEARS_ISYTEM_H

namespace GLib::Internal::Interface
{
    /**
     * @brief 内部システム用インターフェース
     */
    class ISystem
    {
    protected:
        ISystem() = default;
        virtual ~ISystem() = default;
    };
}

#endif // !GEARS_ISYSTEM_H
