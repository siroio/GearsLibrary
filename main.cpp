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
#include <Window.h>
#include <InputSystem.h>

using namespace Glib;

// テスト用コンポーネント
class TestComponent : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable TestComponent");
        GameObject()->Transform()->Position(Vector3{ 1280, 720, 0});
    }

    void Update()
    {
        auto& transform = GameObject()->Transform();
        auto current = GameObject()->Transform()->Position();
        Vector3 veclocity;
        float speed = 10;
        if (InputSystem::GetKey(KeyCode::Up))
        {
            veclocity.y += speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Down))
        {
            veclocity.y -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Left))
        {
            veclocity.x -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Right))
        {
            veclocity.x += speed * GameTimer::DeltaTime();
        }
    }

    void FixedUpdate()
    {

    }
};

// テスト用シーンクラス
class TestScene : public Glib::Scene
{
public:
    void Start() override
    {
        auto& tex = TextureManager::Instance();
        tex.Load(0, "C:\\Users\\rukar\\Pictures\\stone.jpg");
        Debug::Log("Scene Loaded...");

        auto canvas = GameObjectManager::Instantiate("Canvas");
        canvas->AddComponent<Canvas>();
        auto img = GameObjectManager::Instantiate("Img");
        img->Transform()->Parent(canvas->Transform());
        auto imgComp = img->AddComponent<Image>();
        img->AddComponent<TestComponent>();
        imgComp->TextureID(0);                          // 画像をテクスチャ番号0番に設定
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
        SceneManager::Register<TestScene>();
        Debug::Log("Scene: " + SceneManager::SceneName<TestScene>() + " Registered");
        Debug::Log("TestScene Load Start");
        SceneManager::LoadScene("TestScene");
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
