// テスト用実行cpp
#include <iostream>
#include <Game.h>
#include <GameTimer.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Debugger.h>
#include <GameObject.h>
#include <GameObjectManager.h>
#include <Component.h>

#include <TextureManager.h>
#include <Components/Camera.h>
#include <Components/Canvas.h>
#include <Components/Image.h>

using namespace Glib;

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
    void Start() override
    {
        auto& tex = TextureManager::Instance();
        tex.Load(0, "C:\\Users\\rukar\\Pictures\\立ち絵.png");
        Debug::Log("Scene Loaded...");

        auto canvas = GameObjectManager::Instantiate("Canvas");
        canvas->AddComponent<Canvas>();
        auto img = GameObjectManager::Instantiate("Img");
        img->Transform()->Parent(canvas->Transform());
        auto imgComp = img->AddComponent<Image>();
        imgComp->TextureID(0);
        imgComp->Center(Vector2{ 0 });
        imgComp->Color(Color{ 1.0f, 1.0f, 1.0f, 1.0f});
    }

    void End() override
    {
        Debug::Log("Scene Ended...");
    }
};

// ゲーム本体
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
