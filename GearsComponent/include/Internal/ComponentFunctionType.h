#pragma once

/**
 * @brief �R���|�[�l���g�̊֐�
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
