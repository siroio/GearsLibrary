#if _MSC_VER >= 1937 || __cplusplus > 201703L
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include <Game.h>
#include <SystemManager.h>
#include <Internal/ISystem.h>
#include <Internal/MemoryCheck.h>

/* SYSTEM HEADERS */
#include <Internal/XAudioSystem.h>
#include <Internal/PhysXManager.h>
#include <Internal/CameraManager.h>
#include <Internal/ComponentManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/Shader/ShaderManager.h>
#include <Internal/RenderingManager.h>
#include <Internal/EffekseerManager.h>
#include <Internal/CanvasManager.h>
#include <Internal/ImGuiManager.h>
#include <Internal/PhysicsDebugRenderer.h>
#include <AudioManager.h>
#include <GameObjectManager.h>
#include <GameTimer.h>
#include <InputSystem.h>
#include <Random.h>
#include <SceneManager.h>
#include <SkyboxManager.h>

namespace
{
    auto& s_systemManager = SystemManager::Instance();
    float s_accumulatedTime{ 0.0f };
}

int Glib::Game::Run()
{
#if defined(DEBUG) || defined(_DEBUG)
    Internal::MemoryCheck::Start();
#endif
    RegisterSystem();

    if (!Initialize()) return -1;

    Start();

    MSG msg{};
    while (msg.message != WM_QUIT && !isExit)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        Update();
        Draw();
    }

    Finalize();
    End();
    return 0;
}

void Glib::Game::RegisterSystem()
{
    SystemManager::AddSystem<Internal::Graphics::CameraManager>();
    SystemManager::AddSystem<Internal::Graphics::CanvasManager>();
    SystemManager::AddSystem<Internal::ComponentManager>();
    SystemManager::AddSystem<Internal::Graphics::DirectX12>();
    SystemManager::AddSystem<Internal::Physics::PhysXManager>();
    SystemManager::AddSystem<Internal::Graphics::GraphicsResource>();
    SystemManager::AddSystem<Internal::Graphics::RenderingManager>();
    SystemManager::AddSystem<Internal::Graphics::ShaderManager>();
    SystemManager::AddSystem<Internal::Audio::XAudioSystem>();
    SystemManager::AddSystem<Internal::Effect::EffekseerManager>();
    SystemManager::AddSystem<AudioManager>();
    SystemManager::AddSystem<GameObjectManager>();
    SystemManager::AddSystem<GameTimer>();
    SystemManager::AddSystem<InputSystem>();
    SystemManager::AddSystem<Random>();
    SystemManager::AddSystem<SceneManager>();
    SystemManager::AddSystem<SkyboxManager>();

#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ用
    SystemManager::AddSystem<Internal::Debug::ImGuiManager>();
    SystemManager::AddSystem<Internal::Physics::PhysicsDebugRenderer>();
#endif

}

bool Glib::Game::Initialize()
{
    return s_systemManager.Initialize();
}

void Glib::Game::Update()
{
    s_accumulatedTime = std::fminf(
        s_accumulatedTime + GameTimer::DeltaTime(),
        GameTimer::MaximumAllowedTimeStep()
    );

    while (s_accumulatedTime > GameTimer::FixedTimeStep())
    {
        s_systemManager.FixedUpdate();
        s_accumulatedTime -= GameTimer::FixedTimeStep();
    }
    s_systemManager.Update();
}

void Glib::Game::Draw()
{
    s_systemManager.BeginDraw();
    s_systemManager.Draw();
#if defined(DEBUG) || defined(_DEBUG)
    s_systemManager.DebugDraw();
#endif
    s_systemManager.EndDraw();
}

void Glib::Game::Finalize()
{
    s_systemManager.Finalize();
}
