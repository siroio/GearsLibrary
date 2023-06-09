#include <Game.h>
#include <d3d12.h>
#include <SystemManager.h>
#include <GameObjectManager.h>
#include <SceneManager.h>
#include <GameTimer.h>

#include <Internal/ISystem.h>
#include <Internal/ComponentManager.h>

namespace
{
    auto& systemManager = SystemManager::Instance();
}

int Game::Run()
{
    RegisterSystem();
    if (!Initialize()) return -1;

    Start();

    MSG msg = {};
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
    SystemManager::AddSystem<GameObjectManager>();
    SystemManager::AddSystem<GLib::Internal::ComponentManager>();
    SystemManager::AddSystem<GLib::Scene::SceneManager>();
    SystemManager::AddSystem<GameTimer>();
}

bool Game::Initialize()
{
    return systemManager.Initialize();
}

void Game::Update()
{
    systemManager.Update();
}

void Game::Draw()
{
    systemManager.BeginDraw();
    systemManager.Draw();
#if _DEBUG
    systemManager.DebugDraw();
#endif
    systemManager.EndDraw();
}

void Game::Finalize()
{
    systemManager.Finalize();
}
