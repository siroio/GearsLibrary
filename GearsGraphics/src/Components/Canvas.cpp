#include <Components/Canvas.h>
#include <Internal/CanvasManager.h>

namespace
{
    auto s_canvasManager = Glib::Internal::Graphics::CanvasManager::Instance();
}

void Glib::Canvas::Start()
{
    s_canvasManager->AddCanvas(shared_from_this());
}

int Glib::Canvas::Order() const
{
    return order_;
}

void Glib::Canvas::Order(int order)
{
    if (order == order_) return;
    s_canvasManager->ChangeCanvasOrder(shared_from_this(), order);
    order_ = order;
}
