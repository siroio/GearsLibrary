#include <Internal/ComponentEventFunctionList.h>

bool Glib::Internal::ComponentEventFunctionList::Empty() const
{
    return functions_.empty();
}

void Glib::Internal::ComponentEventFunctionList::Execute(FunctionType type, const GameObjectPtr& gameObject)
{
    ExecuteFromVariant<0>(type, gameObject);
}

void Glib::Internal::ComponentEventFunctionList::Execute(FunctionType type, const EventMsg& msg)
{
    ExecuteFromVariant<1>(type, msg);
}
