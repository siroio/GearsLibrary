#ifndef GEARS_COMPONENT_FUNCTION_TYPE_H
#define GEARS_COMPONENT_FUNCTION_TYPE_H

enum class ComponentFunctionType
{
    Start,
    Update,
    LateUpdate,

    OnTriggerEnter,
    OnTriggerStay,
    OnTriggerExit,
    OnCollisionEnter,
    OnCollisionStay,
    OnCollisionExit,
    ReceiveEvent,
};

#endif // GEARS_COMPONENT_FUNCTION_TYPE_H
