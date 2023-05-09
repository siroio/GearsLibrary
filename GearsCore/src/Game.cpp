#include <Game.h>
#include <SystemManager.h>
#include <d3d12.h>

int Game::Run()
{
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

void Game::Exit()
{
    isExit = true;
}

bool Game::Initialize()
{
    return true;
}

void Game::Update()
{}

void Game::Draw()
{}

void Game::Finalize()
{}
