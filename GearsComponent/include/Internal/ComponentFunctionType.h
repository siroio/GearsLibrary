#pragma once

/**
 * @brief コンポーネントの関数
 */
enum class ComponentFunctionType
{
    Start,
    Update,
    FixedUpdate,
    LateUpdate,

    OnTriggerEnter,
    OnTriggerStay,
    OnTriggerExit,
    OnCollisionEnter,
    OnCollisionStay,
    OnCollisionExit,
};

