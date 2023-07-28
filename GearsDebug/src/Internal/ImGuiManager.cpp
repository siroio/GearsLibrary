#include <Internal/ImGuiManager.h>
#include <Internal/ImGuiInc.h>
#include <DX12/Internal/d3dx12Inc.h>
#include <DX12/Internal/DirectX12.h>
#include <DX12/Internal/RenderTarget.h>
#include <Window.h>
#include <Vector2.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto& s_window = Glib::Window::Instance();
    constexpr int NUM_FRAMES_IN_FLIGHT = 3;
}

namespace
{
    Glib::Internal::Graphics::RenderTarget s_renderTarget;
}

bool Glib::Internal::Debug::ImGuiManager::Initialize()
{
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //auto& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //ImGui::StyleColorsDark();
    //auto hwnd = Window::WindowHandle();
    //auto resDesc = dx12_->BackBufferResourceDesc();
    //const Vector2& windowSize = window_.WindowSize();

    //bool result = renderTarget_.Create(
    //    static_cast<UINT>(windowSize.x),
    //    static_cast<UINT>(windowSize.y),
    //    DXGI_FORMAT_R8G8B8A8_UNORM,
    //    dx12_->DescriptorPool(Graphics::DirectX12::POOLTYPE::RES)
    //);
    //if (!result) return false;

    //if (!ImGui_ImplWin32_Init(hwnd)) return false;
    ////TODO: IMGUI SETUP
    //result = ImGui_ImplDX12_Init(
    //    dx12_->Device().Get(),
    //    NUM_FRAMES_IN_FLIGHT,
    //    DXGI_FORMAT_R8G8B8A8_UNORM,
    //    dx12_->DescriptorPool(Graphics::DirectX12::POOLTYPE::RES)->GetHeap().Get(),
    //    renderTarget_.Handle()->CPU(),
    //    renderTarget_.Handle()->GPU()
    //);
    //if (!result) return false;

    //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //srvDesc.Texture2D.MipLevels = 0;

    return true;
}

void Glib::Internal::Debug::ImGuiManager::BeginDraw()
{

}

void Glib::Internal::Debug::ImGuiManager::DebugDraw()
{

}

void Glib::Internal::Debug::ImGuiManager::EndDraw()
{

}

void Glib::Internal::Debug::ImGuiManager::Finalize()
{

}

void Glib::Internal::Debug::ImGuiManager::Log(const std::string message, LogLevel level)
{

}

void Glib::Internal::Debug::ImGuiManager::RenderTarget() const
{

}

bool Glib::Internal::Debug::ImGuiManager::WindowActive(const std::string& name)
{
    return false;
}

void Glib::Internal::Debug::ImGuiManager::CreateConsole()
{

}

void Glib::Internal::Debug::ImGuiManager::CreateDock()
{

}

void Glib::Internal::Debug::ImGuiManager::CreateGameScreen()
{

}
