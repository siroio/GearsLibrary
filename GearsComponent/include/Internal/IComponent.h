#ifndef GEARS_ICOMPONENT_H
#define GEARS_ICOMPONENT_H

class GameObject;

namespace GLib
{
    template<class T>
    class WeakPtr;
}

namespace GLib::Internal::Interface
{
    /**
     * @brief コンポーネントインターフェース
     */
    class IComponent
    {
    protected:
        virtual ~IComponent() = default;

    public:
        virtual void SetGameObject(const GLib::WeakPtr<GameObject>& gameObject) = 0;
    };
}

#endif // !GEARS_ICOMPONENT_H
