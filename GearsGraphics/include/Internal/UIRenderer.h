#pragma once
#include <Component.h>

namespace Glib::Internal
{
    /**
     * @brief 画像表示用基底コンポーネント
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

