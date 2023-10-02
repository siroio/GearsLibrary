#include <Components/Canvas.h>

void Glib::Canvas::Start()
{

}

int Glib::Canvas::Order() const
{
    return order_;
}

void Glib::Canvas::Order(int order)
{
    order_ = order;
}
