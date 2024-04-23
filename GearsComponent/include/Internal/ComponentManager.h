#pragma once
#include <unordered_map>
#include <GameObjectPtr.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
#include <Internal/ISystem.h>
#include <Internal/ComponentFunctionList.h>
#include <Internal/ComponentEventFunctionList.h>
#include <Singleton.h>

namespace Glib::Internal
{
    namespace Interface
    {
        class IComponent;
    }

    /**
     * @brief コンポーネント管理クラス
     */
    class ComponentManager final :
        public Interface::ISystem,
        public SingletonPtr<ComponentManager>,
        public Function::UpdateOrderSet<Order::Update::COMPONENT>
    {
    private:
        friend WeakPtr<ComponentManager> SingletonPtr<ComponentManager>::Instance();
        ComponentManager() = default;

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

    template<class ComponentType, class... Args>
    inline std::shared_ptr<ComponentType> ComponentManager::AddComponent(const GameObjectPtr& gameObject, Args&& ...args)
    {
        std::shared_ptr<ComponentType> component = std::make_shared<ComponentType>(std::forward<Args>(args)...);
        std::static_pointer_cast<Interface::IComponent>(component)->SetGameObject(gameObject);
        normalFunction_.AddFunction(component);
        std::uintptr_t ptr = gameObject.getId();

        if (eventFunction_.contains(ptr))
        {
            eventFunction_[ptr].AddFunction(component);
        }
        else
        {
            ComponentEventFunctionList list{};
            list.AddFunction(component);
            if (!list.Empty()) eventFunction_[ptr] = std::move(list);
        }

        return component;
    }

    template<class... Args>
    inline void ComponentManager::ExecuteNormalFunction(ComponentFunctionType type, const Args&... args)
    {
        normalFunction_.Execute(type, args...);
    }

    template<class... Args>
    inline void ComponentManager::ExecuteEventFunction(const GameObjectPtr& gameObject, ComponentFunctionType type, const Args&... args)
    {
        const auto& function = eventFunction_.find(gameObject.getId());
        if (function == eventFunction_.end()) return;
        function->second.Execute(type, args...);
    }
}
