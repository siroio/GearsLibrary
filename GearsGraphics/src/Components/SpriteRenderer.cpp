#include <Components/SpriteRenderer.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/DX12/DynamicConstantBuffer.h>
#include <Internal/RenderingManager.h>
#include <Internal/CameraBase.h>
#include <TextureManager.h>
#include <GameObject.h>
#include <Mathf.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <GLGUI.h>

using namespace  Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_resource = GraphicsResource::Instance();
    auto s_renderingManager = RenderingManager::Instance();
    auto s_textureManager = Glib::TextureManager::Instance();

    struct SpriteConstant
    {
        Matrix4x4 world{};
        Vector2 center{ 0.5f, 0.5f };
        Vector2 spriteSize{ 0.0f, 0.0f };
        Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct SpriteVertex
    {
        Vector2 position{ 0.0f, 0.0f };
        Vector2 uv{ 0.0f, 0.0f };
    };
}

Glib::SpriteRenderer::SpriteRenderer()
{
    vertexBuffer_.Create(sizeof(SpriteVertex), 4);
}

void Glib::SpriteRenderer::Start()
{
    transform_ = GameObject()->Transform();
    auto renderer = WeakPtr<SpriteRenderer>{ shared_from_this() };
    s_renderingManager->AddRenderer(renderer.get());
}

void Glib::SpriteRenderer::LateUpdate()
{
    if (!enabled_) return;

    // 定数バッファ
    SpriteConstant cbuffer{};
    cbuffer.world = Matrix4x4::TRS(
        transform_->Position(),
        transform_->Rotation(),
        transform_->Scale()
    );
    cbuffer.center = center_;
    cbuffer.spriteSize = clippingSize_ / static_cast<float>(pixelPerUnit_);
    cbuffer.color = color_;

    auto buffer = s_dx12->GetConstantBuffer();
    constantBuffer_ = buffer->Alloc(&cbuffer, sizeof(SpriteConstant));

    //切り取りしたUV 座標計算
    // 左上
    Vector2 leftTopUV = clippingPosition_;
    leftTopUV.x /= textureSize_.x;
    leftTopUV.y /= textureSize_.y;

    // 右下
    Vector2 rightBottomUV;
    rightBottomUV.x = leftTopUV.x + (clippingSize_.x / textureSize_.x);
    rightBottomUV.y = leftTopUV.y + (clippingSize_.y / textureSize_.y);

    // UV反転
    if (flipX_) std::swap(leftTopUV.x, rightBottomUV.x);
    if (flipY_) std::swap(leftTopUV.y, rightBottomUV.y);

    // 頂点バッファ
    SpriteVertex vertices[]
    {
        // position, uv
        { Vector2{ 0.0f, 0.0f }, Vector2{ leftTopUV.x,          leftTopUV.y } },
        { Vector2{ 0.0f,-1.0f }, Vector2{ leftTopUV.x,      rightBottomUV.y } },
        { Vector2{ 1.0f, 0.0f }, Vector2{ rightBottomUV.x,      leftTopUV.y } },
        { Vector2{ 1.0f,-1.0f }, Vector2{ rightBottomUV.x,  rightBottomUV.y } }
    };

    // 頂点バッファ更新
    vertexBuffer_.Update(vertices);
}

void Glib::SpriteRenderer::Draw(const WeakPtr<Internal::CameraBase>& camera)
{
    if (!enabled_) return;

    vertexBuffer_.SetBuffer();
    s_resource->SetPipelineState(ID::SPRITE_PIPELINESTATE);
    camera->SetConstantBuffer(1);
    s_dx12->CommandList()->SetGraphicsRootConstantBufferView(2, constantBuffer_.Address());
    s_textureManager->SetTexture(textureID_, 0);
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    s_dx12->CommandList()->DrawInstanced(4, 1, 0, 0);
}

const Vector2& Glib::SpriteRenderer::Center() const
{
    return center_;
}

void Glib::SpriteRenderer::Center(const Vector2& center)
{
    center_ = center;
}

const Color& Glib::SpriteRenderer::Color() const
{
    return color_;
}

void Glib::SpriteRenderer::Color(const::Color& color)
{
    color_ = color;
}

bool Glib::SpriteRenderer::FlipX() const
{
    return flipX_;
}

void Glib::SpriteRenderer::FlipX(bool flip)
{
    flipX_ = flip;
}

bool Glib::SpriteRenderer::FlipY() const
{
    return flipY_;
}

void Glib::SpriteRenderer::FlipY(bool flip)
{
    flipY_ = flip;
}

const Vector2& Glib::SpriteRenderer::ClippingPosition() const
{
    return clippingPosition_;
}

void Glib::SpriteRenderer::ClippingPosition(const Vector2& position)
{
    clippingPosition_ = position;
}

const Vector2& Glib::SpriteRenderer::ClippingSize() const
{
    return clippingSize_;
}

void Glib::SpriteRenderer::ClippingSize(const Vector2& size)
{
    clippingSize_ = size;
}

unsigned int Glib::SpriteRenderer::PixelPerUnit() const
{
    return pixelPerUnit_;
}

void Glib::SpriteRenderer::PixelPerUnit(unsigned int ppu)
{
    pixelPerUnit_ = ppu;
}

unsigned int Glib::SpriteRenderer::TextureID() const
{
    return textureID_;
}

void Glib::SpriteRenderer::TextureID(unsigned int id, bool isResetSize)
{
    enabled_ = s_textureManager->Contains(id);
    textureID_ = id;
    if (!enabled_) return;
    textureSize_ = s_textureManager->TextureSize(id);
    if (isResetSize) clippingSize_ = textureSize_;
}

void Glib::SpriteRenderer::OnGUI()
{
    int id = static_cast<int>(textureID_);
    if (GLGUI::InputInt("Texture ID", &id))
    {
        TextureID(static_cast<unsigned int>(id));
    }
    int ppu = static_cast<int>(pixelPerUnit_);
    if (GLGUI::InputInt("PixelPerUnit", &ppu))
    {
        pixelPerUnit_ = static_cast<unsigned int>(ppu);
    }
    GLGUI::DragVector2("Center", &center_, 0.01f, 0.0f, 1.0f);
    GLGUI::ColorInput4("Color", &color_);
    GLGUI::DragVector2("Clip Position", &clippingPosition_);
    GLGUI::DragVector2("Clip Size", &clippingSize_);
    GLGUI::Text("Flip");
    GLGUI::CheckBox("X", &flipX_, true);
    GLGUI::CheckBox("Y", &flipY_, true);
}
