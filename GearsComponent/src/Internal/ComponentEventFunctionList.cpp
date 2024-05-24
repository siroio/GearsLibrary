#include <Internal/ComponentEventFunctionList.h>

bool Glib::Internal::ComponentEventFunctionList::Empty() const
{
    return functions_.empty();
}

void Glib::Internal::ComponentEventFunctionList::Execute(FunctionType type, const GameObjectPtr& gameObject)
{
    ExecuteFunction<0>(type, gameObject);
}

void Glib::Internal::ComponentEventFunctionList::Execute(FunctionType type, const EventMsg& msg)
{
    ExecuteFunction<1>(type, msg);
}
