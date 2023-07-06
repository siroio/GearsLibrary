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
#include <ObjectPool.h>
using namespace std;

class TestComponent : public Component
{
public:
    void Start()
    {
        Debug::Log("Start TestComponent");
    }

    void Update()
    {

    }
private:
};

class TestScene : public Glib::Scene::Scene
{
public:
    virtual void Start() override
    {
        Debug::Log("Scene Loaded...");
        auto go = GameObjectManager::Instatiate("TestObject");
        Debug::Log(go->Name());
        auto ptr = go->AddComponent<TestComponent>();
        Debug::Log(ptr->GameObject()->Name());
        Debug::Log("ActiveStatus: " + std::to_string(ptr->Active()));
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
        Glib::Scene::SceneManager::Register<TestScene>();
        Glib::Scene::SceneManager::LoadScene("TestScene");
    }

    virtual void End() override
    {
        cout << "END" << endl;
    }
};

int main()
{
    MyGame{}.Run();
}
