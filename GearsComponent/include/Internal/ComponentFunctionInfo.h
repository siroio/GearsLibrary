#ifndef GEARS_COMPONENT_FUNCTION_INFO_H
#define GEARS_COMPONENT_FUNCTION_INFO_H

#include <WeakPtr.h>

class Component;

namespace GLib::Internal
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
            const GLib::WeakPtr<Component>& component,
            const std::shared_ptr<Interface::IFunc<ReturnType, Args...>>& function)
            : component{ component }, function{ function }
        {}

        GLib::WeakPtr<Component> component;
        std::shared_ptr<Interface::IFunc<ReturnType, Args...>> function;
    };
}

#endif // !GEARS_COMPONENT_FUNCTION_INFO_H
