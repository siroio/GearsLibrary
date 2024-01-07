// テスト用実行cpp
#include <iostream>
#include <Glib.h>
#include <Game.h>
#include <GameTimer.h>
#include <Scene.h>
#include <Debugger.h>

#include <Window.h>
#include <InputSystem.h>
#include <GLObject.h>

#include <TextureManager.h>
#include <GameObjectManager.h>
#include <AnimationManager.h>
#include <SkyboxManager.h>
#include <MeshManager.h>
#include <SceneManager.h>

#include <Internal/ImGuiManager.h>
#include <Internal/RenderingManager.h>

#include <GameObject.h>
#include <Component.h>
#include <Components/Camera.h>
#include <Components/Canvas.h>
#include <Components/Image.h>
#include <Components/SpriteRenderer.h>
#include <Components/MeshRenderer.h>
#include <Components/SkinnedMeshRenderer.h>
#include <Components/Animator.h>

using namespace Glib;

namespace
{
    auto imgui = Internal::Debug::ImGuiManager::Instance();
    auto renderingManager = Internal::Graphics::RenderingManager::Instance();

    const Vector3 LIGHT_DIRECTION{ 59.0f, 169.5f, 0.0f };
    const Color LIGHT_AMBIENT{ 0.7f, 0.7f, 0.7f, 1.0f };
    const Color LIGHT_DIFFUSE{ 0.7f, 0.7f, 0.7f, 1.0f };
}

// テスト用コンポーネント
class TestMover : public Component
{
public:
    void Start()
    {
        imgui->Log("Enable " + nameof(*this));
    }

    void Update()
    {
        auto& transform = GameObject()->Transform();
        Vector3 velocity;
        Vector3 rotation;
        float speed = 10;
        if (InputSystem::GetButton(GPADKey::A))
        {
            imgui->Log("Pressed A");
        }
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
        // アセット読み込み
        bool isloaded = SkyboxManager::Instance()->Load(
            0,
            "Assets/Skybox/DayTop.png",
            "Assets/Skybox/DayBottom.png",
            "Assets/Skybox/DayLeft.png",
            "Assets/Skybox/DayRight.png",
            "Assets/Skybox/DayFront.png",
            "Assets/Skybox/DayBack.png"
        );
        if (!isloaded) return;

        isloaded = MeshManager::Instance().Load(0, R"(Assets/Appearance Miku\Appearance Miku.globj)");
        if (!isloaded) return;
        isloaded = AnimationManager::Instance().Load(0, R"(Assets/上肢テスト01-表情入り.glanim)");
        if (!isloaded) return;

        // 平行光源設定
        renderingManager->LightAmbient(LIGHT_AMBIENT);
        renderingManager->LightDiffuse(LIGHT_DIFFUSE);
        renderingManager->LightDirection(LIGHT_DIRECTION);

        // カメラ作成
        SkyboxManager::Instance()->SetSkybox(0);
        auto camera = GameObjectManager::Instantiate("Camera");
        auto initPosition = Vector3{ 0.0f, 10.0f, -20.0f };
        camera->Transform()->Position(initPosition);
        camera->AddComponent<Camera>()->ClearFlags(CameraClearFlags::SkyBox);

        // オブジェクト生成
        auto mesh = GameObjectManager::Instantiate("Mesh");
        auto renderer = mesh->AddComponent<SkinnedMeshRenderer>();
        auto animator = mesh->AddComponent<Animator>();
        mesh->AddComponent<TestMover>();
        renderer->MeshID(0);
        animator->AnimationID(0);

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

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    MyGame{}.Run();
}
