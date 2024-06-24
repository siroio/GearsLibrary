#include <Components/Image.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/DX12/DynamicConstantBuffer.h>
#include <Internal/CanvasManager.h>
#include <TextureManager.h>
#include <Components/Canvas.h>
#include <GameObject.h>
#include <Window.h>
#include <Mathf.h>
#include <GLGUI.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_graphics = Glib::Internal::Graphics::GraphicsResource::Instance();
    auto s_canvasManager = Glib::Internal::Graphics::CanvasManager::Instance();
    auto s_textureManager = Glib::TextureManager::Instance();

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

void Glib::Image::Start()
{
    auto canvas = GameObject()->GetComponentInParent<Canvas>();
    s_canvasManager->AddUI(WeakPtr<Image>{ shared_from_this() }, canvas);
}

void Glib::Image::LateUpdate()
{
    if (!enabled_) return;
    const auto& transform = GameObject()->Transform();
    ImageConstant cbuffer;
    cbuffer.position = transform->Position();
    cbuffer.center = center_;
    cbuffer.scale = transform->Scale();
    cbuffer.angle = transform->EulerAngles().z * Mathf::DEG2RAD;
    cbuffer.textureSize = s_textureManager->TextureSize(textureID_);
    cbuffer.windowSize = Window::WindowSize();
    cbuffer.color = color_;
    auto buffer = s_dx12->GetConstantBuffer();
    constantBuffer_ = buffer->Alloc(&cbuffer, sizeof(ImageConstant));
}

void Glib::Image::DrawUI()
{
    if (!enabled_) return;
    using namespace Internal::Graphics;
    s_graphics->SetPipelineState(ID::IMAGE_PIPELINESTATE);
    s_graphics->SetVertexBuffer(ID::IMAGE_VERTEX);
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(1, constantBuffer_.Address());
    s_textureManager->SetTexture(textureID_, 0);
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
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
    enabled_ = s_textureManager->Contains(id);
}

void Glib::Image::OnGUI()
{
    Component::OnGUI();

    int texID = static_cast<int>(textureID_);
    if (GLGUI::InputInt("TextureID", &texID))
    {
        TextureID(static_cast<unsigned int>(texID));
    }
    GLGUI::DragVector2("Center", &center_, 0.01f, 0.0f, 1.0f);
    GLGUI::ColorPicker4("Color", &color_);
}
