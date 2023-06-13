// テスト用実行cpp
#include <iostream>
#include <Game.h>
#include <GameTimer.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Logger.h>
using namespace std;

class TestScene : public GLib::Scene::Scene
{
public:
    virtual void Start() override
    {

    }

    virtual void End() override
    {

    }
};

class MyGame : public Game
{
    virtual void Start() override
    {
        Debug::Log("GAME STARTED. . . . .");
        GLib::Scene::SceneManager::Register<TestScene>();
    }

    virtual void End() override
    {
        cout << "END" << endl;
    }
};

auto main() -> int
{
    MyGame{}.Run();
}
