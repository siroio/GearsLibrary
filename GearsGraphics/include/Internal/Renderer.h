#pragma once
#include <Component.h>

namespace Glib::Internal
{
    /**
     * @brief �`��p���R���|�[�l���g
     */
    class Renderer : public Component
    {
    protected:
        Renderer() = default;
        virtual ~Renderer() = default;
    };
}
