#include <Internal/RenderingManager.h>
#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Internal/DX12/ConstantBuffer.h>

#include <Matrix4x4.h>
#include <Color.h>
#include <Vector3.h>
#include <Vector2.h>

namespace
{
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();
    Glib::Internal::Graphics::ConstantBuffer s_cBuffer{};

    Color s_ambient{ 0.2f, 0.2f, 0.2f };
    Color s_diffuse{ 1.0f, 1.0f, 1.0f };
    Color s_specular{ 1.0f, 1.0f, 1.0f };
    Vector3 s_direction{ 1.0f, -1.0f, 1.0f };

    float s_shadowBias{ 0.001f };
    float s_shadowNear{ 0.1f };
    float s_shadowFar{ 100.0f };
    float s_shadowDistance{ 100.0f };
    Vector2 s_shadowRange{ 25.0f, 25.0f };
}

namespace Glib::Internal::Graphics
{
    struct DirectionalLightConstant
    {
        Color ambient;
        Color diffuse;
        Color specular;
        Vector3 direction;
        float shadowBias{ 0.005f };
    };
}

bool Glib::Internal::Graphics::RenderingManager::Initialize()
{
    return s_cBuffer.Create(sizeof(DirectionalLightConstant));
}

void Glib::Internal::Graphics::RenderingManager::Update()
{
    // 定数バッファの更新
    DirectionalLightConstant cBuffer{};
    cBuffer.ambient = s_ambient;
    cBuffer.diffuse = s_diffuse;
    cBuffer.specular = s_specular;
    cBuffer.direction = s_direction;
    cBuffer.shadowBias = s_shadowBias;
    s_cBuffer.Update(sizeof(cBuffer), &cBuffer);
}

void Glib::Internal::Graphics::RenderingManager::Draw()
{
    // 無効なコンポーネントをリストから削除
    for (auto& [type, component] : renderers_)
    {
        component.remove_if([](const auto& info)
        {
            return info.function->IsDelete();
        });
    }

    // 有効なカメラの数だけ描画
    for (const auto& camera : s_cameraManager->Cameras())
    {
        if (!camera->Active()) continue;

        // シャドウマップ描画
        camera->SetDepthStencil();
        for (const auto& info : renderers_.at(DrawType::Shadow))
        {
            if (info.component->Active())
            {
                info.function->Call(camera);
            }
        }

        // オブジェクトの描画
        for (const auto& info : renderers_.at(DrawType::Draw))
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
    return s_direction;
}

void Glib::Internal::Graphics::RenderingManager::LightDirection(const Vector3& direction)
{
    s_direction = direction;
}

Matrix4x4 Glib::Internal::Graphics::RenderingManager::CalculateMatrixForShadowMap(const Vector3& gazePoint) const
{
    Vector3 lightPosition = gazePoint - (s_direction.Normalized() * (s_shadowFar - s_shadowNear) / 2.0f);
    return Matrix4x4::LookAt(lightPosition, gazePoint, Vector3::Up()) *
        Matrix4x4::Orthographic(s_shadowRange.x, s_shadowRange.y, s_shadowNear, s_shadowFar);
}

void Glib::Internal::Graphics::RenderingManager::ShadowMapBias(float bias)
{
    s_shadowBias = bias;
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
    s_cBuffer.BindPipeline(rootParamIndex);
}
