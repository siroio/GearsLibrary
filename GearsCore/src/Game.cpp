#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <fstream>
#endif
#include <Game.h>
#include <SystemManager.h>
#include <Internal/ISystem.h>
#include <DX12/Internal/DirectX12.h>
#include <Internal/ComponentManager.h>
#include <GameObjectManager.h>
#include <SceneManager.h>
#include <InputSystem.h>
#include <GameTimer.h>

namespace
{
    auto& s_systemManager = SystemManager::Instance();
}

int Game::Run()
{
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
