#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <fstream>
#endif
#include <Game.h>
#include <SystemManager.h>
#include <Internal/ISystem.h>
#include <Internal/DirectX12.h>
#include <Internal/ComponentManager.h>
#include <GameObjectManager.h>
#include <SceneManager.h>
#include <InputSystem.h>
#include <GameTimer.h>

namespace
{
    auto& systemManager = SystemManager::Instance();
}

int MemoryLeakHandler(int reportType, char* message, int* returnValue)
{
    std::ofstream logFile{ "leakCheck.txt", std::ios_base::app };
    if (logFile.is_open())
    {
        logFile << message << std::endl;
        logFile.close();
    }
    return 0;
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
#if defined(DEBUG) || defined(_DEBUG)
    systemManager.DebugDraw();
#endif
    systemManager.EndDraw();
}

void Game::Finalize()
{
    systemManager.Finalize();
}
