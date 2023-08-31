#pragma once
#include <Component.h>

class Transform;

namespace Glib
{

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
