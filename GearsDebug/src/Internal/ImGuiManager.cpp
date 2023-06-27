#include <Internal/ImGuiManager.h>
#include <Internal/ImGuiInc.h>
#include <Internal/DirectX12.h>
#include <Window.h>
#include <Vector2.h>

namespace
{
    auto dx12_ = Glib::Internal::Graphics::DirectX12::Instance();
    auto& window_ = Glib::Window::Instance();
}

bool Glib::Internal::Debug::ImGuiManager::Initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
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
