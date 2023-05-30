#ifndef GEARS_COMPONENT_MANAGER_H
#define GEARS_COMPONENT_MANAGER_H

#include <GameObjectPtr.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
#include <Internal/ISystem.h>
#include <Internal/IComponent.h>
#include <Internal/ComponentFunctionList.h>
#include <Internal/ComponentEventFunctionList.h>
#include <Singleton.h>

#include <unordered_map>

namespace GLib::Internal
{
    namespace Interface
    {
        class IComponent;
    }

    class ComponentManager :
        public Interface::ISystem,
        public GLib::Utility::SingletonPtr<ComponentManager>,
        public GLib::Internal::Function::UpdateOrderSet<GLib::Constant::Order::Update::COMPONENT>,
        public GLib::Internal::Function::FinalizeOrderSet<GLib::Constant::Order::Finalize::COMPONENT>
    {
    public:
        void Update();
        void Finalize();

        template<class ComponentType, class... Args>
        std::shared_ptr<ComponentType> AddComponent(const GameObjectPtr& gameObject, Args&&... args);

        template<class... Args>
        void ExecuteNormalFunction(ComponentFunctionType type, const Args&... args);

        template<class... Args>
        void ExecuteEventFunction(const GameObjectPtr& gameObject, ComponentFunctionType type, const Args&... args);

    private:
        ComponentFunctionList normalFunction_;
        std::unordered_map<std::uintptr_t, ComponentEventFunctionList> eventFunction_;
    };

    template<class ComponentType, class ...Args>
    inline std::shared_ptr<ComponentType> ComponentManager::AddComponent(const GameObjectPtr& gameObject, Args&& ...args)
    {
        auto component = std::make_shared<ComponentType>(args...);
        std::static_pointer_cast<Interface::IComponent>(component)->SetGameObject(gameObject);

        std::uintptr_t ptr = gameObject.getId();

        if (eventFunction_.find(ptr) == eventFunction_.end())
        {
            ComponentEventFunctionList list = {};
            list.AddFunction(component);
            if (!list.Empty()) eventFunction_[ptr] = list;
        }
        else
        {
            eventFunction_[ptr].AddFunction(component);
        }

        return component;
    }

    template<class ...Args>
    inline void ComponentManager::ExecuteNormalFunction(ComponentFunctionType type, const Args&... args)
    {
        normalFunction_.Execute(type, args);
    }

    template<class ...Args>
    inline void ComponentManager::ExecuteEventFunction(const GameObjectPtr& gameObject, ComponentFunctionType type, const Args&... args)
    {
        const std::uintptr_t ptr = gameObject.getId();
        if (eventFunction_.find(ptr) == eventFunction_.end()) return;
        eventFunction_[ptr].Execute(type, args...);
    }
}

#endif // !GEARS_COMPONENT_H
