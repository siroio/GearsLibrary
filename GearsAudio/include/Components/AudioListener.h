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
         * @brief �ʒu�v�Z
         */
        void LateUpdate();

    private:
        WeakPtr<Transform> transform_{ nullptr };
    };
}
