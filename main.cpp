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
#include <AnimationManager.h>
#include <Internal/ImGuiManager.h>

using namespace Glib;

namespace
{
    auto imgui = Internal::Debug::ImGuiManager::Instance();
}

// テスト用コンポーネント
class TestMover : public Component
{
public:
    void Start()
    {
        imgui->Log("Enable " + nameof<TestMover>());
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
class TestScene : public Scene
{
public:
    void Start() override
    {
        if (!MeshManager::Instance().Load(0, R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\Appearance Miku\Appearance Miku.globj)"))
        {
            return;
        }
        if (!AnimationManager::Instance().Load(0, R"(C:\Users\rukar\Desktop\MikuMikuDance_v932x64\いもができたモーション\にぼミクさん版\にぼミクさん_いもができた.glanim)"))
        {
            return;
        }

        auto mesh = GameObjectManager::Instantiate("Mesh");
        auto renderer = mesh->AddComponent<SkinnedMeshRenderer>();
        auto animator = mesh->AddComponent<Animator>();
        mesh->AddComponent<TestMover>();
        renderer->MeshID(0);
        animator->AnimationID(0);

        auto camera = GameObjectManager::Instantiate("Camera");
        auto initPosition = Vector3{ 0.0f, 3.0f, -20.0f };
        camera->Transform()->Position(initPosition);
        camera->AddComponent<Camera>()->ClearFlags(CameraClearFlags::Color);

        imgui->Log("Scene Loading...");
    }

    void End() override
    {
        imgui->Log("Scene End...");
    }
};

// ゲーム本体
class MyGame : public Game
{
    void Start() override
    {
        imgui->Log("GAME STARTTING");
        SceneManager::Register<TestScene>();
        imgui->Log("Scene: " + SceneManager::SceneName<TestScene>() + " Registered");
        imgui->Log("TestScene Load Start");
        SceneManager::LoadScene("TestScene");
        imgui->Log("TestScene Load Complete");
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
