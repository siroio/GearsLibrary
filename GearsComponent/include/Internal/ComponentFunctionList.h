#pragma once
#include <variant>
#include <list>
#include <unordered_map>
#include <Internal/ComponentFunctionInfo.h>
#include <Internal/ComponentFunctionType.h>
#include <FuncDefinition.h>
#include <string>

namespace Glib
{
    template<class T>
    class WeakPtr;
}

namespace Glib::Internal
{
    class CameraBase;

    class ComponentFunctionList
    {
        template<class ReturnType, class... Args>
        using FunctionInfo = ComponentFunctionInfo<ReturnType, Args...>;

        using FunctionVariant = std::variant<FunctionInfo<void>, FunctionInfo<void, Glib::WeakPtr<CameraBase>>>;
        using FunctionType = ComponentFunctionType;
    public:
        void Update();
        void Clear();

        void Execute(FunctionType type);
        void Execute(FunctionType type, const Glib::WeakPtr<CameraBase>& camera);

        void ExecuteClear(FunctionType type);
        void ExecuteClear(FunctionType type, const Glib::WeakPtr<CameraBase>& camera);

        template<class T> requires std::derived_from<T, Component>
        void AddFunction(const std::shared_ptr<T>& component);

    private:
        void Remove(FunctionType type);

        template<size_t index, class... Args>
        void ExecuteFromVariant(FunctionType type, const Args&... args);

        template<class T> requires HasStartFunc<T, void>
        void AddStart(const std::shared_ptr<T>& component);
        void AddStart(...)
        {}

        template<class T> requires HasUpdateFunc<T, void>
        void AddUpdate(const std::shared_ptr<T>& component);
        void AddUpdate(...)
        {}

        template<class T> requires HasFixedUpdateFunc<T, void>
        void AddFixedUpdate(const std::shared_ptr<T>& component);
        void AddFixedUpdate(...)
        {}

        template<class T> requires HasLateUpdateFunc<T, void>
        void AddLateUpdate(const std::shared_ptr<T>& component);
        void AddLateUpdate(...)
        {}

    private:
        std::list<std::pair<FunctionType, FunctionVariant>> addedFunction_;
        std::unordered_map<FunctionType, std::list<FunctionVariant>> functions_;
    };

    template<class T> requires std::derived_from<T, Component>
    inline void ComponentFunctionList::AddFunction(const std::shared_ptr<T>& component)
    {
        AddStart(component);
        AddUpdate(component);
        AddFixedUpdate(component);
        AddLateUpdate(component);
    }

    template<size_t index, class ...Args>
    inline void ComponentFunctionList::ExecuteFromVariant(FunctionType type, const Args& ...args)
    {
        for (const auto& variant : functions_[type])
        {
            if (variant.index() != index) continue;
            const auto& info = std::get<index>(variant);
            bool isCall = !info.component.expired() && info.component->Active();
            if (isCall)
            {
                info.function->Call(args...);
            }
        }
    }

    template<class T> requires HasStartFunc<T, void>
    inline void ComponentFunctionList::AddStart(const std::shared_ptr<T>& component)
    {
        addedFunction_.emplace_back(
            FunctionType::Start,
            FunctionInfo<void>{ component, std::make_shared<Function::HasStartObject<T, void>>(component)}
        );
    }

    template<class T> requires HasUpdateFunc<T, void>
    inline void ComponentFunctionList::AddUpdate(const std::shared_ptr<T>& component)
    {
        addedFunction_.emplace_back(
            FunctionType::Update,
            FunctionInfo<void>{ component, std::make_shared<Function::HasUpdateObject<T, void>>(component) }
        );
    }

    template<class T> requires HasFixedUpdateFunc<T, void>
    inline void ComponentFunctionList::AddFixedUpdate(const std::shared_ptr<T>& component)
    {
        addedFunction_.emplace_back(
            FunctionType::FixedUpdate,
            FunctionInfo<void>{ component, std::make_shared<Function::HasFixedUpdateObject<T, void>>(component) }
        );
    }

    template<class T> requires HasLateUpdateFunc<T, void>
    inline void ComponentFunctionList::AddLateUpdate(const std::shared_ptr<T>& component)
    {
        addedFunction_.emplace_back(
            FunctionType::LateUpdate,
            FunctionInfo<void>{ component, std::make_shared<Function::HasLateUpdateObject<T, void>>(component) }
        );
    }
}
