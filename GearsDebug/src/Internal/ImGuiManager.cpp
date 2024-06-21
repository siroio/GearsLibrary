#include <Internal/ImGuiManager.h>
#include <Internal/ImGuiInc.h>
#include <Internal/ImGuiFont.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DirectX12.h>
#include <ComPtr.h>
#include <Color.h>
#include <Vector2.h>
#include <RenderTarget.h>
#include <TimeUtility.h>
#include <GameTimer.h>
#include <Debugger.h>
#include <filesystem>
#include <unordered_map>
#include <list>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto& s_window = Glib::Window::Instance();
    constexpr int NUM_FRAMES_IN_FLIGHT = 3;
    std::unordered_map<Glib::LogLevel, ImVec4> s_logTextColor;
}

namespace
{
    Glib::Graphics::RenderTarget s_renderTarget;
    std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle> s_renderTargetSRV;
    std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle> s_imguiResource;

    Color s_clearColor{ Color::Black() };
    D3D12_RECT s_scissorRect{};
    D3D12_VIEWPORT s_viewport{};
    std::list<Glib::Internal::Debug::LogMessage> s_consoleLog;
    bool s_enableGameView{ true };
    bool s_enableConsole{ true };
    bool s_resetLayout{ false };
    std::string s_fontPath{ R"(C:/Windows/Fonts/meiryo.ttc)" };
}

bool Glib::Internal::Debug::ImGuiManager::Initialize()
{
    s_window.RegisterProcedure(this);
    auto hwnd = Window::WindowHandle();
    const Vector2& windowSize = Window::WindowSize();
    auto resPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RES);

    // Setup Heaps
    s_imguiResource = resPool->GetHandle();

    // Setup Rendertarget
    s_renderTarget.Create(
        static_cast<UINT64>(windowSize.x),
        static_cast<UINT64>(windowSize.y),
        s_clearColor,
        DXGI_FORMAT_R8G8B8A8_UNORM
    );

    // RenderTarget用SRV
    s_renderTargetSRV = resPool->GetHandle();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    s_dx12->Device()->CreateShaderResourceView(
        s_renderTarget.RenderTargetResource().Get(),
        &srvDesc, s_renderTargetSRV->CPU()
    );

    // IMGUIの初期化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImFontConfig fontConfig;
    fontConfig.MergeMode = false;

    if (std::filesystem::exists(s_fontPath))
    {
        io.Fonts->AddFontFromFileTTF(
            s_fontPath.data(),
            18,
            &fontConfig,
            ImGuiGlyphRangesJapanese
        );
    }

    ImGui::StyleColorsDark();

    if (!ImGui_ImplWin32_Init(hwnd)) return false;
    if (!ImGui_ImplDX12_Init(
        s_dx12->Device().Get(),
        NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        resPool->GetHeap().Get(),
        s_imguiResource->CPU(),
        s_imguiResource->GPU()
        )) return false;

    s_scissorRect.top = 0;
    s_scissorRect.left = 0;
    s_scissorRect.right = static_cast<UINT>(windowSize.x);
    s_scissorRect.bottom = static_cast<UINT>(windowSize.y);

    s_viewport.TopLeftX = 0.0f;
    s_viewport.TopLeftY = 0.0f;
    s_viewport.Width = windowSize.x;
    s_viewport.Height = windowSize.y;
    s_viewport.MinDepth = 0.0f;
    s_viewport.MaxDepth = 1.0f;

    SetGUIStyle();

    // レイアウトをリセットするか
    s_resetLayout =
        io.IniFilename == nullptr ||
        io.IniFilename == "" ||
        !std::filesystem::exists(io.IniFilename);

    s_logTextColor.emplace(LogLevel::Info, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    s_logTextColor.emplace(LogLevel::Warn, ImVec4{ 1.0f, 0.843f, 0.0f, 1.0f });
    s_logTextColor.emplace(LogLevel::Error, ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f });

    return true;
}

void Glib::Internal::Debug::ImGuiManager::BeginDraw()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    s_renderTarget.AsRenderTarget();

    s_dx12->CommandList()->ClearRenderTargetView(
        s_renderTarget.RTVHandle()->CPU(),
        s_clearColor.Raw(),
        0, nullptr
    );
}

void Glib::Internal::Debug::ImGuiManager::DebugDraw()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Window"))
    {
        if (ImGui::Button("Reset Layout")) s_resetLayout = true;
        ImGui::Checkbox("GameView", &s_enableGameView);
        ImGui::Checkbox("Console", &s_enableConsole);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    CreateDock();
    if (s_enableGameView) DrawGameView();
    if (s_enableConsole) DrawConsole();
}

void Glib::Internal::Debug::ImGuiManager::EndDraw()
{
    s_renderTarget.AsTexture();
    s_dx12->SetDefaultRenderTarget();
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), s_dx12->CommandList().Get());

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault(nullptr, s_dx12->CommandList().Get());
}

void Glib::Internal::Debug::ImGuiManager::Finalize()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    s_renderTargetSRV.reset();
    s_imguiResource.reset();
    s_renderTarget.Release();
    s_consoleLog.clear();
}

void Glib::Internal::Debug::ImGuiManager::Log(std::string_view message, LogLevel loglevel)
{
    std::string msg;
    msg.append(Glib::TimeUtility::CurrentTimeStr());
    msg.append(Glib::Debug::GetPrefix(loglevel));
    msg.append(" ");
    msg.append(message);
    msg.append("\n");
    Debug::LogMessage log;
    log.level = loglevel;
    log.message = msg;
    s_consoleLog.push_back(log);
}

void Glib::Internal::Debug::ImGuiManager::SetRenderTarget() const
{
    s_dx12->CommandList()->OMSetRenderTargets(1, &s_renderTarget.RTVHandle()->CPU(), false, nullptr);
    s_dx12->CommandList()->RSSetScissorRects(1, &s_scissorRect);
    s_dx12->CommandList()->RSSetViewports(1, &s_viewport);
}

bool Glib::Internal::Debug::ImGuiManager::WindowActive(std::string_view windowName)
{
    ImGuiWindow* window = ImGui::FindWindowByName(windowName.data());
    return window != nullptr && window->Active;
}

void Glib::Internal::Debug::ImGuiManager::DrawConsole()
{
    ImGui::Begin("Console", &s_enableConsole);
    if (ImGui::Button("Clear")) s_consoleLog.clear();
    ImGui::SameLine();
    ImGui::Text("FPS: %d", static_cast<int>(1.0f / GameTimer::UnscaledDeltaTime()));
    ImGui::Separator();

    ImGui::BeginChild("Message");
    for (const auto& [level, message] : s_consoleLog)
    {
        ImVec4 textoColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        if (auto colorIt = s_logTextColor.find(level); colorIt != s_logTextColor.end())
        {
            textoColor = colorIt->second;
        }
        ImGui::TextColored(textoColor, message.c_str());
    }

    // auto scroll
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}

void Glib::Internal::Debug::ImGuiManager::CreateDock()
{
    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiDockNodeFlags_NoCloseButton;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::Begin("MainDockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar();

    // レイアウトの初期化
    if (s_resetLayout) ResetLayout();

    auto mainDockID = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(mainDockID, ImVec2{ 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    ImGui::End();
}

void Glib::Internal::Debug::ImGuiManager::DrawGameView()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("GameWindow", &s_enableGameView);
    ImGui::PopStyleVar();

    ImVec2 regionSize = ImGui::GetContentRegionAvail();
    Vector2 viewSize = Vector2{ regionSize.x, regionSize.y };
    float viewAspect = viewSize.x / viewSize.y;

    Vector2 windowSize = Window::WindowSize();
    float windowAspect = windowSize.x / windowSize.y;

    Vector2 size{ viewSize };

    if (viewAspect >= windowAspect) size.x = viewSize.y * windowAspect;
    else size.y = viewSize.x / windowAspect;

    auto curPos = ImGui::GetCursorPos();
    Vector2 pos = Vector2{ curPos.x, curPos.y };

    ImGui::SetCursorPos(ImVec2{ pos.x, pos.y });
    ImGui::Image(static_cast<ImTextureID>(s_renderTargetSRV->GPU().ptr), ImVec2{ size.x, size.y });
    ImGui::End();
}

void Glib::Internal::Debug::ImGuiManager::ResetLayout()
{
    s_resetLayout = false;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiID dockSpaceID = ImGui::GetID("MainDockSpace");
    ImGui::DockBuilderRemoveNode(dockSpaceID);
    ImGui::DockBuilderAddNode(dockSpaceID, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockSpaceID, viewport->WorkSize);
    ImGuiID gameViewDockID{ 0 };
    ImGuiID consoleDockID{ 0 };
    ImGuiID propertiesDockID{ 0 };
    ImGuiID hierarchyDockID{ 0 };

    ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Right, 0.2f, &propertiesDockID, &gameViewDockID);
    ImGui::DockBuilderSplitNode(gameViewDockID, ImGuiDir_Right, 0.2f, &hierarchyDockID, &gameViewDockID);
    ImGui::DockBuilderSplitNode(gameViewDockID, ImGuiDir_Down, 0.3f, &consoleDockID, &gameViewDockID);

    ImGui::DockBuilderDockWindow("Properties", propertiesDockID);
    ImGui::DockBuilderDockWindow("Console", consoleDockID);
    ImGui::DockBuilderDockWindow("Hierarchy", hierarchyDockID);
    ImGui::DockBuilderDockWindow("GameWindow", gameViewDockID);
    ImGui::DockBuilderFinish(dockSpaceID);
}

void Glib::Internal::Debug::ImGuiManager::SetGUIStyle()
{
    ImVec4* const colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25.00f;
    style.ScrollbarSize = 15.00f;
    style.GrabMinSize = 10.00f;
    style.WindowBorderSize = 1.00f;
    style.ChildBorderSize = 1.00f;
    style.PopupBorderSize = 1.00f;
    style.FrameBorderSize = 1.00f;
    style.TabBorderSize = 1.00f;
    style.WindowRounding = 7.00f;
    style.ChildRounding = 4.00f;
    style.FrameRounding = 3.00f;
    style.PopupRounding = 4.00f;
    style.ScrollbarRounding = 9.00f;
    style.GrabRounding = 3.00f;
    style.LogSliderDeadzone = 4.00f;
    style.TabRounding = 4.00f;
    style.WindowMenuButtonPosition = ImGuiDir_None;
}


void Glib::Internal::Debug::ImGuiManager::operator()(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
}
