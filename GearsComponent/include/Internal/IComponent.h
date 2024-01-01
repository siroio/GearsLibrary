#pragma once
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

