#ifndef GEARS_COMPONENT_MANAGER_H
#define GEARS_COMPONENT_MANAGER_H

#include <Internal/ISystem.h>
#include <Internal/IComponent.h>
#include <Singleton.h>
#include <GameObjectPtr.h>

namespace GLib::Internal
{
    namespace Interface
    {
        class IComponent;
    }

    class ComponentManager :
        public Interface::ISystem,
        public GLib::Utility::SingletonPtr<ComponentManager>
    {
    public:
        void Update();
        void Finalize();

        template<class ComponentType, class... Args>
        std::shared_ptr<ComponentType> AddComponent(const GameObjectPtr& gameObject, Args&&... args);

        template<class... Args>
        void ExecuteNormalFunction();

        template<class... Args>
        void ExecuteEventFunction();

    private:

    };

    template<class ComponentType, class ...Args>
    inline std::shared_ptr<ComponentType> ComponentManager::AddComponent(const GameObjectPtr& gameObject, Args && ...args)
    {
        auto component = std::make_shared<ComponentType>(args...);
        std::static_pointer_cast<Interface::IComponent>(component)->SetGameObject(gameObject);

        std::uintptr_t ptr = gameObject.getId();

        return component;
    }

    template<class ...Args>
    inline void ComponentManager::ExecuteNormalFunction()
    {}

    template<class ...Args>
    inline void ComponentManager::ExecuteEventFunction()
    {}
}

#endif // !GEARS_COMPONENT_H
