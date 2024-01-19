#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/ImGuiManager.h>
namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_imgui = Glib::Internal::Debug::ImGuiManager::Instance();
}

void Glib::Internal::Graphics::CameraManager::BeginDraw()
{
    cameras_.remove_if([](const auto& camera)
    {
        return camera.expired();
    });
}

void Glib::Internal::Graphics::CameraManager::Draw()
{
    // RenderTargetの設定
#if defined(DEBUG) || defined(_DEBUG)
    // TODO: デバッグ用のRenderTargetへ描画
    s_imgui->SetRenderTarget();
#else
    s_dx12->SetDefaultRenderTarget();
#endif
    for (const auto& camera : cameras_)
    {
        // カメラが無効の場合は描画しない
        if (!camera->Active()) continue;
        camera->Draw();
    }
}

void Glib::Internal::Graphics::CameraManager::AddCamera(const WeakPtr<CameraBase>& camera)
{
    cameras_.push_back(camera);
}

const std::list<Glib::WeakPtr<Glib::Internal::CameraBase>> Glib::Internal::Graphics::CameraManager::Cameras() const
{
    return cameras_;
}
