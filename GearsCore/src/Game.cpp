﻿#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <Game.h>
#include <SystemManager.h>
#include <Internal/ISystem.h>

/* SYSTEM HEADERS */
#include <Internal/XAudioSystem.h>
#include <Internal/JoltPhysicsManager.h>
#include <Internal/CameraManager.h>
#include <Internal/ComponentManager.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/Shader/ShaderManager.h>
#include <Internal/RenderingManager.h>
#include <Internal/EffekseerManager.h>
#include <Internal/CanvasManager.h>
#include <Internal/ImGuiManager.h>
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
}

int Glib::Game::Run()
{
#if defined(DEBUG) || defined(_DEBUG)
    // メモリリーク検出開始
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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
    SystemManager::AddSystem<Internal::Graphics::GraphicsResource>();
    SystemManager::AddSystem<Internal::Graphics::RenderingManager>();
    SystemManager::AddSystem<Internal::Graphics::ShaderManager>();
    SystemManager::AddSystem<Internal::Audio::XAudioSystem>();
    SystemManager::AddSystem<Internal::Effect::EffekseerManager>();
    SystemManager::AddSystem<Internal::Physics::JoltPhysicsManager>();
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
#endif // !

}

bool Glib::Game::Initialize()
{
    return s_systemManager.Initialize();
}

void Glib::Game::Update()
{
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
