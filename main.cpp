// �e�X�g�p���scpp
#include <iostream>
#include <Game.h>
#include <GameTimer.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Debugger.h>
#include <GameObject.h>
#include <GameObjectManager.h>
#include <Component.h>

using namespace Glib;

// �e�X�g�p�R���|�[�l���g
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

// �e�X�g�p�V�[���N���X
class TestScene : public Glib::Scene::Scene
{
public:
    void Start() override
    {
        Debug::Log("Scene Loaded...");
        auto go = Glib::GameObjectManager::Instantiate("TestObject");
        if (!go.expired())Debug::Log("TestObject Created!");
        auto ptr = go->AddComponent<TestComponent>();
        Debug::Log(Glib::ToString(go->Name()) + "ActiveStatus: " + std::to_string(go->Active()));
    }

    void End() override
    {
        Debug::Log("Scene Ended...");
    }
};

// �Q�[���{��
class MyGame : public Glib::Game
{
    void Start() override
    {
        Debug::Log("GAME STARTTING");
        Glib::Scene::SceneManager::Register<TestScene>();
        Debug::Log("Scene: " + Glib::Scene::SceneManager::SceneName<TestScene>() + " Registered");
        Debug::Log("TestScene Load Start");
        Glib::Scene::SceneManager::LoadScene("TestScene");
        Debug::Log("TestScene Load Complete");
    }

    void End() override
    {
        std::cout << "GAME END" << std::endl;
    }
};

int main()
{
    MyGame{}.Run();
}
