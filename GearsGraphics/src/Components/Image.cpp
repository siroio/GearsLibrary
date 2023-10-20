#include <Components/Image.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/CanvasManager.h>
#include <TextureManager.h>
#include <Components/Canvas.h>
#include <GameObject.h>
#include <Window.h>
#include <Mathf.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_graphics = Glib::Internal::Graphics::GraphicsResource::Instance();
    auto s_canvasManager = Glib::Internal::Graphics::CanvasManager::Instance();
    auto& s_textureManager = Glib::TextureManager::Instance();

    struct ImageConstant
    {
        Vector2 position{ 0.0f, 0.0f };
        Vector2 center{ 0.5f, 0.5f };
        Vector2 scale{ 1.0f, 1.0f };
        float angle{ 0.0f };
        float padding{ 0.0f };
        Vector2 textureSize{ 0.0f, 0.0f };
        Vector2 windowSize{ 1240.0f, 720.0f };
        Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
    };
}

Glib::Image::Image()
{
    constantBuffer_.Create(sizeof(ImageConstant));
}

void Glib::Image::Start()
{
    auto canvas = GameObject()->GetComponentInParent<Canvas>();
    s_canvasManager->AddUI(shared_from_this(), canvas);
}

void Glib::Image::LateUpdate()
{
    if (!enabled_) return;
    const auto& transform = GameObject()->Transform();
    ImageConstant buffer;
    buffer.position = transform->Position();
    buffer.center = center_;
    buffer.scale = transform->Scale();
    buffer.angle = transform->EulerAngles().z * Mathf::DEG2RAD;
    buffer.textureSize = s_textureManager.TextureSize(textureID_);
    buffer.windowSize = Window::WindowSize();
    buffer.color = color_;
    constantBuffer_.Update(sizeof(buffer), &buffer);
}

void Glib::Image::DrawUI()
{
    if (!enabled_) return;
    using namespace Internal::Graphics;
    s_graphics->SetPipelineState(ID::IMAGE_PIPELINESTATE);
    s_graphics->SetVertexBuffer(ID::IMAGE_VERTEX);
    constantBuffer_.SetBuffer(1);
    s_textureManager.SetTexture(textureID_, 0);
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    s_dx12->CommandList()->DrawInstanced(4, 1, 0, 0);
}

const Vector2& Glib::Image::Center() const
{
    return center_;
}

void Glib::Image::Center(const Vector2& center)
{
    center_ = center;
}

const Color& Glib::Image::Color() const
{
    return color_;
}

void Glib::Image::Color(const::Color& color)
{
    color_ = color;
}

unsigned int Glib::Image::TextureID() const
{
    return textureID_;
}

void Glib::Image::TextureID(unsigned int id)
{
    textureID_ = id;
    enabled_ = s_textureManager.Contains(id);
}
