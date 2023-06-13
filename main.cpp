// テスト用実行cpp
#include <iostream>
#include <Game.h>
#include <GameTimer.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Logger.h>
#include <GameObject.h>
#include <GameObjectManager.h>
#include <Component.h>
using namespace std;

class TestComponent : public Component
{
public:
    void Start()
    {
        Debug::Log("Start");
    }
    void Update()
    {
        frameCount++;
        system("cls");
        Debug::Log("Test Update" + frameCount);
    }
    void LateUpdate()
    {
        Debug::Log("up");
    }
private:
    int frameCount = 0;
};

class TestScene : public GLib::Scene::Scene
{
public:
    virtual void Start() override
    {
        Debug::Log("Scene Loaded...");
        auto go = GameObjectManager::Instatiate("TestObject");
        Debug::Log(go->Name());
        go->AddComponent<TestComponent>();
    }

    virtual void End() override
    {
        Debug::Log("Scene Ended...");
    }
};

class MyGame : public Game
{
    virtual void Start() override
    {
        Debug::Log("GAME STARTED. . . . .");
        GLib::Scene::SceneManager::Register<TestScene>();
        GLib::Scene::SceneManager::LoadScene("TestScene");
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
