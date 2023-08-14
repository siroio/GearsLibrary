#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <Game.h>
#include <SystemManager.h>
#include <Internal/ISystem.h>

#include <Internal/ComponentManager.h>
#include <DX12/Internal/DirectX12.h>
#include <GameObjectManager.h>
#include <GameTimer.h>
#include <InputSystem.h>
#include <Random.h>
#include <SceneManager.h>

namespace
{
    auto& s_systemManager = SystemManager::Instance();
}

int Game::Run()
{
#if defined(DEBUG) | defined(_DEBUG)
    // メモリリーク検出開始
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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

void Game::RegisterSystem()
{
    SystemManager::AddSystem<Glib::Internal::ComponentManager>();
    SystemManager::AddSystem<Glib::Internal::Graphics::DirectX12>();
    SystemManager::AddSystem<GameObjectManager>();
    SystemManager::AddSystem<GameTimer>();
    SystemManager::AddSystem<Glib::InputSystem>();
    SystemManager::AddSystem<Glib::Random>();
    SystemManager::AddSystem<Glib::Scene::SceneManager>();
}

bool Game::Initialize()
{
    return s_systemManager.Initialize();
}

void Game::Update()
{
    s_systemManager.Update();
}

void Game::Draw()
{
    s_systemManager.BeginDraw();
    s_systemManager.Draw();
#if defined(DEBUG) || defined(_DEBUG)
    s_systemManager.DebugDraw();
#endif
    s_systemManager.EndDraw();
}

void Game::Finalize()
{
    s_systemManager.Finalize();
}
