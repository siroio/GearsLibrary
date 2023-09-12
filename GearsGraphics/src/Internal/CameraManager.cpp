#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Internal/DX12/DirectX12.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

void Glib::Internal::Graphics::CameraManager::BeginDraw()
{
    cameras_.remove_if([](const auto& cam)
    {
        return cam.expired();
    });
}

void Glib::Internal::Graphics::CameraManager::Draw()
{
    // RenderTarget‚ÌÝ’è
#if defined(DEBUG) || defined(_DEBUG)

#else
    s_dx12->SetDefaultRenderTarget();
#endif
    for (const auto& camera : cameras_)
    {
        // ƒJƒƒ‰‚ª–³Œø‚Ìê‡‚Í•`‰æ‚µ‚È‚¢
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
