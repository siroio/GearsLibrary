#ifndef GEARS_ICOMPONENT_H
#define GEARS_ICOMPONENT_H

class GameObject;

namespace GLib::Utility
{
    template<class T>
    class WeakPtr;
}

namespace GLib::Internal::Interface
{
    class IComponent
    {
    protected:
        virtual ~IComponent() = default;

    public:
        virtual void SetGameObject(const GLib::Utility::WeakPtr<GameObject>& gameObject) = 0;
    };
}

#endif // !GEARS_ICOMPONENT_H
