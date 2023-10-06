#pragma once
#include <Component.h>

namespace Glib::Internal
{
    /**
     * @brief 描画用基底コンポーネント
     */
    class Renderer : public Component
    {
    protected:
        Renderer() = default;
        virtual ~Renderer() = default;
    };
}
