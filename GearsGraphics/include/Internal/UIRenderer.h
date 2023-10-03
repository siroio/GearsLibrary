#pragma once
#include <Component.h>

namespace Glib::Internal
{
    /**
     * @brief �摜�\���p���R���|�[�l���g
     */
    class UIRenderer : public Component
    {
    protected:
        UIRenderer() = default;
        virtual ~UIRenderer() = default;

    public:
        virtual void DrawUI() = 0;
    };
}
