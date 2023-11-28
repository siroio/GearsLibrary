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
#include <MeshManager.h>
#include <Internal/RenderingManager.h>
#include <Components/Camera.h>
#include <Components/Canvas.h>
#include <Components/Image.h>
#include <Components/MeshRenderer.h>
#include <Window.h>
#include <InputSystem.h>
#include <GLObject.h>

using namespace Glib;

// テスト用コンポーネント
class TestMover : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable TestComponent");
    }

    void Update()
    {
        auto& transform = GameObject()->Transform();
        Vector3 velocity;
        Vector3 rotation;
        float speed = 10;
        if (InputSystem::GetKey(KeyCode::Up))
        {
            velocity.y -= speed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::Down))
        {
            velocity.y += speed * GameTimer::DeltaTime();
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
        if (!MeshManager::Instance().Load(0, "D:/MyLib/GearsLibrary/初音ミクver.2.1/output.globj"))
        {
            Debug::Error("ロード失敗");
        }

        auto render = Internal::Graphics::RenderingManager::Instance();
        render->LightAmbient(Color{ 1.0f, 1.0f, 1.0f, 1.0f });
        render->LightDiffuse(Color{ 1.0f, 1.0f, 1.0f });
        render->LightSpecular(Color{ 1.0f, 1.0f, 1.0f });
        render->LightDirection(Vector3{ 0.0f, -1.0f, 0.0f });

        auto mesh = GameObjectManager::Instantiate("Mesh");
        auto renderer = mesh->AddComponent<MeshRenderer>();

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
    //std::vector<Glib::GLObject::Vertex> quadVertices = {
    //    // 頂点データ (位置, 法線, UV座標, ボーンインデックス, ボーンウェイト, 接線)
    //    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    //    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    //    {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    //    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    //};

    //std::vector<unsigned int> quadIndices = {
    //    0, 1, 2, 0, 2, 3 // 頂点インデックス
    //};

    //std::vector<Glib::GLObject::Subset> quadSubsets = {
    //    { 0, 5, 0 }
    //};

    //std::vector<Glib::GLObject::Material> quadMaterials = {
    //    {
    //        {0.2f, 0.2f, 0.2f, 1.0f}, // 環境光
    //        {1.0f, 1.0f, 1.0f, 1.0f}, // 拡散光
    //        {0.0f, 0.0f, 0.0f, 1.0f}, // 鏡面反射光
    //        0.0f,                      // 光沢度
    //        "texture.png",              // テクスチャファイル名
    //        "normal.png"                // 法線マップファイル名
    //    }
    //};

    //std::vector<Glib::GLObject::Bone> quadBones = {
    //    { "Root", { 0.0f, 0.0f, 0.0f }, -1 }
    //};

    //GLObject quadObject(quadVertices, quadIndices, quadSubsets, quadMaterials, quadBones);

    //quadObject.WriteFile("Quad.globj");
}
