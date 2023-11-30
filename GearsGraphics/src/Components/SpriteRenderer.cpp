#include <Components/SpriteRenderer.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/RenderingManager.h>
#include <Internal/CameraBase.h>
#include <TextureManager.h>
#include <GameObject.h>
#include <Mathf.h>
#include <Vector3.h>

using namespace  Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = DirectX12::Instance();
    auto s_resource = GraphicsResource::Instance();
    auto s_renderingManager = RenderingManager::Instance();
    auto& s_textureManager = Glib::TextureManager::Instance();

    struct SpriteConstant
    {
        Vector3 position{ 0.0f, 0.0f, 0.0f };
        float angle{ 0.0f };
        Vector2 scale{ 1.0f, 1.0f };
        Vector2 center{ 0.5f, 0.5f };
        Vector2 spriteSize{ 0.0f, 0.0f };
        float padding[2]{ 0.0f, 0.0f };
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
    constantBuffer_.Create(sizeof(SpriteConstant));
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
    SpriteConstant buffer{};
    buffer.position = transform_->Position();
    buffer.angle = transform_->EulerAngles().z * Mathf::DEG2RAD;
    buffer.scale = transform_->Scale();
    buffer.center = center_;
    buffer.spriteSize = clippingSize_ / 100.0f;
    buffer.color = color_;
    constantBuffer_.Update(sizeof(buffer), &buffer);

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
    camera->SetConstantBuffer(1);
    s_resource->SetPipelineState(ID::SPRITE_PIPELINESTATE);
    constantBuffer_.SetBuffer(2);
    s_textureManager.SetTexture(textureID_, 0);

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

unsigned int Glib::SpriteRenderer::TextureID() const
{
    return textureID_;
}

void Glib::SpriteRenderer::TextureID(unsigned int id, bool isResetSize)
{
    enabled_ = s_textureManager.Contains(id);
    if (!enabled_) return;
    textureSize_ = s_textureManager.TextureSize(id);
    if (isResetSize) clippingSize_ = textureSize_;
}
