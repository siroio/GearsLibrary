#pragma once
#include <Internal/ICamera.h>
#include <Component.h>

namespace Glib::Internal
{
    /**
     * @brief �J�����R���|�[�l���g�̃x�[�X�N���X
     */
    class CameraBase : public Component, public Interface::ICamera
    {
    protected:
        CameraBase() = default;
        virtual ~CameraBase() = default;
    };
}
