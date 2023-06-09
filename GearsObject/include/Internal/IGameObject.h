#ifndef GEARS_IGAME_OBJECT_H
#define GEARS_IGAME_OBJECT_H

namespace GLib::Internal::Interface
{
    /**
     * @brief ゲームオブジェクトインターフェース
     */
    class IGameObject
    {
    protected:
        IGameObject() = default;
        virtual ~IGameObject() = default;

    public:
        virtual void Initialize() = 0;
    };
}

#endif // !GEARS_IGAME_OBJECT_H
