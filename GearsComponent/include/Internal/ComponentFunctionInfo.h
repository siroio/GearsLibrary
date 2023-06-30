#pragma once
#include <WeakPtr.h>

class Component;

namespace Glib::Internal
{
    namespace Interface
    {
        template<class ReturnType, class... Args>
        class IFunc;
    }

    template<class ReturnType, class... Args>
    struct ComponentFunctionInfo
    {
        ComponentFunctionInfo(
            const Glib::WeakPtr<Component>& component,
            const std::shared_ptr<Interface::IFunc<ReturnType, Args...>>& function)
            : component{ component }, function{ function }
        {}

        Glib::WeakPtr<Component> component;
        std::shared_ptr<Interface::IFunc<ReturnType, Args...>> function;
    };
}
