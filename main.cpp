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
#include <InputSystem.h>


// テスト用コンポーネント
class TestComponent : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable TestComponent");
    }

    void Update()
    {

    }

    void FixedUpdate()
    {

    }
};

// テスト用シーンクラス
class TestScene : public Glib::Scene::Scene
{
public:
    virtual void Start() override
    {
        Debug::Log("Scene Loaded...");
        auto go = GameObjectManager::Instantiate("TestObject");
        if (!go.expired())Debug::Log("TestObject Created!");
        auto ptr = go->AddComponent<TestComponent>();
        Debug::Log(Glib::as_string(go->Name()) + "ActiveStatus: " + std::to_string(ptr->Active()));
    }

    virtual void End() override
    {
        Debug::Log("Scene Ended...");
    }
};

// ゲーム本体
class MyGame : public Game
{
    virtual void Start() override
    {
        Debug::Log("GAME STARTTING");
        Glib::Scene::SceneManager::Register<TestScene>();
        Debug::Log("Scene: " + Glib::Scene::SceneManager::SceneName<TestScene>() + " Registered");
        Debug::Log("TestScene Load Start");
        Glib::Scene::SceneManager::LoadScene("TestScene");
        Debug::Log("TestScene Load Complete");
    }

    virtual void End() override
    {
        std::cout << "GAME END" << std::endl;
    }
};

int main()
{
    MyGame{}.Run();
}
