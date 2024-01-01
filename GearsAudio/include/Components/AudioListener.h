#pragma once
#include <Component.h>

namespace Glib
{
    class Transform;

    class AudioListener : public Component
    {
    public:
        void Start();

        /**
         * @brief 位置計算
         */
        void LateUpdate();

    private:
        WeakPtr<Transform> transform_{ nullptr };
    };
}

