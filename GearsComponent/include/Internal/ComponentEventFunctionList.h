#pragma once
#include <variant>
#include <deque>
#include <unordered_map>
#include <Component.h>
#include <Internal/ComponentFunctionInfo.h>
#include <Internal/ComponentFunctionType.h>
#include <FuncDefinition.h>

namespace Glib
{
    template<class T>
    class WeakPtr;
}

class EventParamt;

namespace Glib::Internal
{
    class ComponentEventFunctionList
    {
        template<class ReturnType, class... Args>
        using FunctionInfo = ComponentFunctionInfo<ReturnType, Args...>;
        using FunctionVariant = std::variant<
            FunctionInfo<void, GameObjectPtr>,
            FunctionInfo<void, EventParamt>>;
        using FunctionType = ComponentFunctionType;

    public:
        bool Empty() const;

        void Execute(FunctionType type, const GameObjectPtr& gameObject);

        template<class T> requires std::derived_from<T, Component>
        void AddFunction(const std::shared_ptr<T>& component);

    private:
        template<size_t index, class... Args>
        void ExecuteFromVariant(FunctionType type, const Args&... args);

        template<class T> requires HasOnTriggerEnterFunc<T, void, GameObjectPtr>
        void AddOnTriggerEnter(const std::shared_ptr<T>& component);
        void AddOnTriggerEnter(...)
        {}

        template<class T> requires HasOnTriggerStayFunc<T, void, GameObjectPtr>
        void AddOnTriggerStay(const std::shared_ptr<T>& component);
        void AddOnTriggerStay(...)
        {}

        template<class T> requires HasOnTriggerExitFunc<T, void, GameObjectPtr>
        void AddOnTriggerExit(const std::shared_ptr<T>& component);
        void AddOnTriggerExit(...)
        {}

        template<class T> requires HasOnCollisionEnterFunc<T, void, GameObjectPtr>
        void AddOnCollisionEnter(const std::shared_ptr<T>& component);
        void AddOnCollisionEnter(...)
        {}

        template<class T> requires HasOnCollisionStayFunc<T, void, GameObjectPtr>
        void AddOnCollisionStay(const std::shared_ptr<T>& component);
        void AddOnCollisionStay(...)
        {}

        template<class T> requires HasOnCollisionExitFunc<T, void, GameObjectPtr>
        void AddOnCollisionExit(const std::shared_ptr<T>& component);
        void AddOnCollisionExit(...)
        {}

    private:
        std::unordered_map<FunctionType, std::deque<FunctionVariant>> functions_;
    };

    template<class T> requires std::derived_from<T, Component>
    inline void ComponentEventFunctionList::AddFunction(const std::shared_ptr<T>& component)
    {
        AddOnTriggerEnter(component);
        AddOnTriggerStay(component);
        AddOnTriggerExit(component);
        AddOnCollisionEnter(component);
        AddOnCollisionStay(component);
        AddOnCollisionExit(component);
    }

    template<std::size_t index, class ...Args>
    inline void ComponentEventFunctionList::ExecuteFromVariant(FunctionType type, const Args& ...args)
    {
        for (const auto& variant : functions_[type])
        {
            if (variant.index() != index) continue;
            const auto& info = std::get<index>(variant);
            if (info.component->Active())
            {
                info.function->Call(args...);
            }
        }
    }

    template<class T> requires HasOnTriggerEnterFunc<T, void, GameObjectPtr>
    inline void ComponentEventFunctionList::AddOnTriggerEnter(const std::shared_ptr<T>& component)
    {
        functions_[FunctionType::OnTriggerEnter].push_back(
            FunctionInfo<void, GameObjectPtr>{ component, std::make_shared<Function::HasOnTriggerEnterObject<T, void, GameObjectPtr>>(component)}
        );
    }

    template<class T> requires HasOnTriggerStayFunc<T, void, GameObjectPtr>
    inline void ComponentEventFunctionList::AddOnTriggerStay(const std::shared_ptr<T>& component)
    {
        functions_[FunctionType::OnTriggerStay].push_back(
            FunctionInfo<void, GameObjectPtr>{ component, std::make_shared<Function::HasOnTriggerStayObject<T, void, GameObjectPtr>>(component)}
        );
    }

    template<class T> requires HasOnTriggerExitFunc<T, void, GameObjectPtr>
    inline void ComponentEventFunctionList::AddOnTriggerExit(const std::shared_ptr<T>& component)
    {
        functions_[FunctionType::OnTriggerExit].push_back(
            FunctionInfo<void, GameObjectPtr>{ component, std::make_shared<Function::HasOnTriggerExitObject<T, void, GameObjectPtr>>(component)}
        );
    }

    template<class T> requires HasOnCollisionEnterFunc<T, void, GameObjectPtr>
    inline void ComponentEventFunctionList::AddOnCollisionEnter(const std::shared_ptr<T>& component)
    {
        functions_[FunctionType::OnCollisionEnter].push_back(
            FunctionInfo<void, GameObjectPtr>{ component, std::make_shared<Function::HasOnCollisionEnterObject<T, void, GameObjectPtr>>(component)}
        );
    }

    template<class T> requires HasOnCollisionStayFunc<T, void, GameObjectPtr>
    inline void ComponentEventFunctionList::AddOnCollisionStay(const std::shared_ptr<T>& component)
    {
        functions_[FunctionType::OnCollisionStay].push_back(
            FunctionInfo<void, GameObjectPtr>{ component, std::make_shared<Function::HasOnCollisionStayObject<T, void, GameObjectPtr>>(component)}
        );
    }

    template<class T> requires HasOnCollisionExitFunc<T, void, GameObjectPtr>
    inline void ComponentEventFunctionList::AddOnCollisionExit(const std::shared_ptr<T>& component)
    {
        functions_[FunctionType::OnCollisionExit].push_back(
            FunctionInfo<void, GameObjectPtr>{ component, std::make_shared<Function::HasOnCollisionExitObject<T, void, GameObjectPtr>>(component)}
        );
    }
}

