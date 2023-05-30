#include <Internal/ComponentEventFunctionList.h>

bool GLib::Internal::ComponentEventFunctionList::Empty() const
{
    return functions_.empty();
}

void GLib::Internal::ComponentEventFunctionList::Execute(FunctionType type, const GameObjectPtr& gameObject)
{
    ExecuteFromVariant<0>(type, gameObject);
}
