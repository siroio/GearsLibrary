#include <Game.h>
#include <SystemManager.h>
#include <d3d12.h>
#include <Internal/ISystem.h>
#include <SystemManager.h>
#include <GameObjectManager.h>

namespace
{
    auto& systemManager = SystemManager::Instance();
    using namespace GLib::Utility;
    using namespace GLib::Internal;
}

class TestManager :
    public SingletonPtr<TestManager>,
    public Interface::ISystem
{
public:
    bool Initialize()
    {
        puts("テスト用マネージャークラスのInitializeが呼ばれました。");
        return true;
    }
};

int Game::Run()
{
    AddSystem();
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

void Game::AddSystem()
{
    SystemManager::AddSystem<TestManager>();
    SystemManager::AddSystem<GameObjectManager>();
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
