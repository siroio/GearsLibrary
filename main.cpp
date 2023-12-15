// テスト用実行cpp
#include <iostream>
#include <Glib.h>
#include <Game.h>
#include <GameTimer.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Debugger.h>
#include <GameObject.h>
#include <GameObjectManager.h>
#include <Component.h>
#include <TextureManager.h>
#include <MeshManager.h>
#include <Internal/RenderingManager.h>
#include <Components/Camera.h>
#include <Components/Canvas.h>
#include <Components/Image.h>
#include <Components/SpriteRenderer.h>
#include <Components/MeshRenderer.h>
#include <Components/SkinnedMeshRenderer.h>
#include <Window.h>
#include <InputSystem.h>
#include <GLObject.h>
#include <Components/Animator.h>

using namespace Glib;

// テスト用コンポーネント
class TestMover : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable " + Glib::nameof<TestMover>());
    }

    void Update()
    {
        auto& transform = GameObject()->Transform();
        Vector3 velocity;
        Vector3 rotation;
        float speed = 10;
        if (InputSystem::GetKey(KeyCode::Up))
        {
            velocity.y += speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Down))
        {
            velocity.y -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Left))
        {
            velocity.x -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Right))
        {
            velocity.x += speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::W))
        {
            velocity.z -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::S))
        {
            velocity.z += speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::A))
        {
            rotation.y -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::D))
        {
            rotation.y += speed * GameTimer::DeltaTime();
        }
        transform->Position(transform->Position() + velocity);
        transform->Rotate(rotation);
    }
};

// テスト用シーンクラス
class TestScene : public Glib::Scene
{
public:
    void Start() override
    {
        if (!Glib::MeshManager::Instance().Load(0, R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\Appearance Miku\Appearance Miku.globj)"))
        {
            return;
        }

        auto light = Internal::Graphics::RenderingManager::Instance();
        light->LightAmbient(Color{ 1.0f, 1.0f, 1.0f, 1.0f });
        light->LightDiffuse(Color{ 1.0f, 1.0f, 1.0f });
        light->LightSpecular(Color{ 1.0f, 1.0f, 1.0f });
        light->LightDirection(Vector3{ 0.0f, -1.0f, 0.0f });

        auto mesh = GameObjectManager::Instantiate("Mesh");
        auto renderer = mesh->AddComponent<SkinnedMeshRenderer>();
        auto animator = mesh->AddComponent<Animator>();
        mesh->AddComponent<TestMover>();

        renderer->MeshID(0);

        auto camera = GameObjectManager::Instantiate("Camera");
        auto initPosition = Vector3{ 0.0f, 0.0f, -10.0f };

        camera->Transform()->Position(initPosition);
        camera->AddComponent<Camera>()->ClearFlags(Glib::CameraClearFlags::Color);

        Debug::Log("Scene Loaded...");
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
