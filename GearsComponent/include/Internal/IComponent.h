#ifndef GEARS_ICOMPONENT_H
#define GEARS_ICOMPONENT_H

class GameObject;

namespace Glib
{
    template<class T>
    class WeakPtr;
}

namespace Glib::Internal::Interface
{
    /**
     * @brief コンポーネントインターフェース
     */
    class IComponent
    {
    protected:
        virtual ~IComponent() = default;

    public:
        virtual void SetGameObject(const Glib::WeakPtr<GameObject>& gameObject) = 0;
    };
}

#endif // !GEARS_ICOMPONENT_H
