#pragma once
#include <Component.h>

namespace Glib
{
    class Canvas : public Component
    {
    public:
        void Start();

        int Order() const;
        void Order(int order);

    private:
        int order_{ 0 };
    };
}
