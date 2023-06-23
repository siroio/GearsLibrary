#include <Internal/DirectX12.h>
#include <Window.h>
#include <Color.h>
#include <ComPtr.h>

namespace
{
    Color bg_color{ 0.0f };
    Glib::Window& window_ = Glib::Window::Instance();
}

bool Glib::Internal::Graphics::DirectX12::Initialize()
{
    if (!window_.Initialize()) return false;
    return true;
}

void Glib::Internal::Graphics::DirectX12::BeginDraw()
{}

void Glib::Internal::Graphics::DirectX12::EndDraw()
{}

void Glib::Internal::Graphics::DirectX12::Finalize()
{}

const Color& Glib::Internal::Graphics::DirectX12::BackGroundColor()
{
    return bg_color;
}

void Glib::Internal::Graphics::DirectX12::BackGroundColor(const Color& color)
{}
