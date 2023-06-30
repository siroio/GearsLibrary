#pragma once

/**
 * @brief �R���|�[�l���g�̊֐�
 */
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
