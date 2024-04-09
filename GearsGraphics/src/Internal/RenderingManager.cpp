#include <Internal/RenderingManager.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/DX12/DirectX12.h>
#include <Matrix4x4.h>
#include <Color.h>
#include <Vector3.h>
#include <Vector2.h>
#include <ranges>
#include <imgui.h>
#include <GLGUI.h>

namespace
{
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();

    Glib::Internal::Graphics::ConstantBuffer s_constantBuffer{};

    Color s_ambient{ 1.0f, 1.0f, 1.0f };
    Color s_diffuse{ 1.0f, 1.0f, 1.0f };
    Color s_specular{ 1.0f, 1.0f, 1.0f };
    Vector3 s_lightDirection{ 1.0f, -1.0f, 1.0f };

    float s_shadowBias{ 0.0001f };
    float s_momentBias{ 0.0000f };
    float s_shadowNear{ 0.1f };
    float s_shadowFar{ 100.0f };
    Vector2 s_shadowRange{ 25.0f };

    // 平行光源用定数バッファ構造体
    struct DirectionalLightConstant
    {
        Color ambient;
        Color diffuse;
        Color specular;
        Vector3 direction;
        float shadowBias{ 0.000f };
        float momentBias{ 0.000f };
        Vector3 padding;
    };
}

bool Glib::Internal::Graphics::RenderingManager::Initialize()
{
    return s_constantBuffer.Create(sizeof(DirectionalLightConstant));
}

void Glib::Internal::Graphics::RenderingManager::Update()
{
    // 定数バッファの更新
    DirectionalLightConstant buffer;
    buffer.ambient = s_ambient;
    buffer.diffuse = s_diffuse;
    buffer.specular = s_specular;
    buffer.direction = s_lightDirection;
    buffer.shadowBias = s_shadowBias;
    buffer.momentBias = s_momentBias;
    s_constantBuffer.Update(sizeof(DirectionalLightConstant), &buffer);
}

void Glib::Internal::Graphics::RenderingManager::Draw()
{
    // 無効なレンダラーをリストから削除
    for (auto& renderer : renderers_ | std::ranges::views::values)
    {
        renderer.remove_if([](const auto& info)
        {
            return info.function->IsDelete();
        });
    }

    // 有効なカメラだけ描画
    for (const auto& camera : s_cameraManager->Cameras())
    {
        if (!camera->Active()) continue;

        // シャドウマップ描画
        camera->SetDepthStencil();
        for (const auto& info : renderers_[DrawType::Shadow])
        {
            if (info.component->Active())
            {
                info.function->Call(camera);
            }
        }

        // シャドウマップにブラーをかける
        camera->ExecuteShadowBlur();

        // オブジェクトの描画
        camera->SetRenderTarget();
        for (const auto& info : renderers_[DrawType::Draw])
        {
            if (info.component->Active())
            {
                info.function->Call(camera);
            }
        }
    }
}

const Color& Glib::Internal::Graphics::RenderingManager::LightAmbient()
{
    return s_ambient;
}

void Glib::Internal::Graphics::RenderingManager::LightAmbient(const Color& color)
{
    s_ambient = color;
}

const Color& Glib::Internal::Graphics::RenderingManager::LightDiffuse()
{
    return s_diffuse;
}

void Glib::Internal::Graphics::RenderingManager::LightDiffuse(const Color& color)
{
    s_diffuse = color;
}

const Color& Glib::Internal::Graphics::RenderingManager::LightSpecular()
{
    return s_specular;
}

void Glib::Internal::Graphics::RenderingManager::LightSpecular(const Color& specular)
{
    s_specular = specular;
}

const Vector3& Glib::Internal::Graphics::RenderingManager::LightDirection()
{
    return s_lightDirection;
}

void Glib::Internal::Graphics::RenderingManager::LightDirection(const Vector3& direction)
{
    s_lightDirection = direction;
}

Matrix4x4 Glib::Internal::Graphics::RenderingManager::CalculateMatrixForShadowMap(const Vector3& cameraGaze) const
{
    Vector3 lightPosition = cameraGaze - (s_lightDirection.Normalized() * (s_shadowFar - s_shadowNear) * 0.5f);
    return Matrix4x4::LookAt(lightPosition, cameraGaze, Vector3::Up()) *
        Matrix4x4::Orthographic(s_shadowRange.x, s_shadowRange.y, s_shadowNear, s_shadowFar);
}

void Glib::Internal::Graphics::RenderingManager::ShadowMapBias(float bias)
{
    s_shadowBias = bias;
}

void Glib::Internal::Graphics::RenderingManager::ShadowMapMomentBias(float bias)
{
    s_momentBias = bias;
}

void Glib::Internal::Graphics::RenderingManager::ShadowMapClip(float nearZ, float farZ)
{
    s_shadowNear = nearZ;
    s_shadowFar = farZ;
}

void Glib::Internal::Graphics::RenderingManager::ShadowMapRange(const Vector2& range)
{
    s_shadowRange = range;
}

void Glib::Internal::Graphics::RenderingManager::SetDirectionalLightConstant(unsigned int rootParamIndex)
{
    s_constantBuffer.SetBuffer(rootParamIndex);
}
