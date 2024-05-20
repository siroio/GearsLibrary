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
#include <AudioManager.h>
#include <EffectManager.h>
#include <GLGUI.h>

#include <GameObject.h>
#include <Component.h>
#include <Components/DirectionalLight.h>
#include <Components/Camera.h>
#include <Components/Canvas.h>
#include <Components/Image.h>
#include <Components/SpriteRenderer.h>
#include <Components/MeshRenderer.h>
#include <Components/SkinnedMeshRenderer.h>
#include <Components/Animator.h>
#include <Components/AudioListener.h>
#include <Components/AudioSource.h>
#include <Components/EffectSystem.h>
#include <Components/Rigidbody.h>
#include <Components/BoxCollider.h>
#include <Components/CapsuleCollider.h>
#include <Components/MeshCollider.h>
#include <filesystem>
#include <unordered_map>


using namespace Glib;

namespace
{
    const Vector3 LIGHT_DIRECTION{ 50.0f, -30.0f, 0.0f };
    const Color LIGHT_AMBIENT{ 0.7f, 0.7f, 0.7f, 1.0f };
    const Color LIGHT_DIFFUSE{ 0.7f, 0.7f, 0.7f, 1.0f };
    const Color LIGHT_SPECULAR{ 0.1f, 0.1f, 0.1f, 0.1f };
}

// テスト用コンポーネント
class TestMover : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable " + nameof<TestMover>());
        rigidbody_ = GameObject()->GetComponent<Rigidbody>();
        camera_ = GameObjectManager::Find("Camera")->GetComponent<Transform>();
    }

    void Update()
    {
        moveDir = Vector3::Zero();
        float speed = 4.0f;

        if (InputSystem::GetButtonDown(GPADKey::B))
        {
            Debug::Log("Press B");
        }

        if (InputSystem::GetKey(KeyCode::W))
        {
            moveDir.z++;
        }
        if (InputSystem::GetKey(KeyCode::S))
        {
            moveDir.z--;
        }
        if (InputSystem::GetKey(KeyCode::A))
        {
            moveDir.x--;
        }
        if (InputSystem::GetKey(KeyCode::D))
        {
            moveDir.x++;
        }

        auto lstick = InputSystem::GetLeftStick();
        if (lstick.SqrMagnitude() > 0.1f)
        {
            moveDir = Vector3{ lstick.x, 0.0f, lstick.y };
        }
        // カメラを基準にしてプレイヤーを動かす
        // カメラの向きに対して移動方向を調整
        moveDir = moveDir.Normalized() * speed;
    }

    void FixedUpdate()
    {
        Vector3 curVelocity = rigidbody_->LinearVelocity();
        Vector3 velocityChange = moveForceMultiplier * (moveDir - Vector3{ curVelocity.x, 0.0f, curVelocity.z });
        rigidbody_->AddForce(velocityChange);
    }

    void OnCollisionEnter(const GameObjectPtr& gameObject)
    {
        Debug::Log(gameObject->Name());
    }

private:
    WeakPtr<Rigidbody> rigidbody_;
    WeakPtr<Transform> camera_;
    Vector3 moveDir{};
    float moveForceMultiplier{ 8.0f };
};

class TestAudio : public Component
{
public:
    void Start()
    {
        auto source = GameObject()->AddComponent<AudioSource>();
        source->AudioID(0);
        source->SetGroup(0);
    }

    void Update()
    {
        auto se = GameObject()->GetComponent<AudioSource>();
        if (InputSystem::GetKeyDown(KeyCode::Space))
        {
            se->Play();
            GameObject()->Destroy();
        }
    }
};

class Tester : public Component
{
public:
    enum class TestType
    {
        SkinnedMesh,
        Effect,
        Sprite,
        Audio
    };

public:
    void Start()
    {
        SetupMesh();
        SetupEfk();
        SetupSprite();
        SetupAudio();
    }

    void Update()
    {
        for (auto& [type, gameObject] : testObjects_)
        {
            if (gameObject.expired())
            {
                testObjects_.erase(type);
                continue;
            }
            gameObject->Active(type == currenType_);
        }
    }

    void ReceiveMsg(const EventMsg& msg)
    {
        if (msg.ValidateType<std::string>())
        {
            Debug::Log(std::to_string(msg.MsgID()) + " : " + msg.Msg<std::string>());
        }
    }

private:
    void SetupMesh()
    {
        auto mesh = GameObjectManager::Instantiate("Mesh");
        mesh->Transform()->Parent(GameObject()->Transform());
        mesh->Transform()->LocalPosition(Vector3{ 0.0f, 2.0f, 1.0f });
        mesh->Transform()->LocalEulerAngles(Vector3{ 0.0f, 140.0f, 0.0f });
        mesh->Transform()->LocalScale(Vector3{ 1.0f, 1.0f, 1.0f });
        mesh->AddComponent<TestMover>();
        mesh->AddComponent<Rigidbody>()->Constraints(RigidbodyConstraints::FreezeRotation);
        auto renderer = mesh->AddComponent<SkinnedMeshRenderer>();
        auto animator = mesh->AddComponent<Animator>();
        auto cap = mesh->AddComponent<CapsuleCollider>();
        renderer->MeshID(1);
        animator->AnimationID(0);
        animator->Loop(true);
        cap->IsVisible(true);
        cap->Height(0.37f);
        cap->Radius(0.38f);
        cap->Center(Vector3{ 0.0f, 0.75f, 0.0f });
        mesh->Active(false);

        testObjects_.emplace(TestType::SkinnedMesh, mesh);
    }

    void SetupEfk()
    {
        auto effect = GameObjectManager::Instantiate("Effect");
        effect->Transform()->Parent(GameObject()->Transform());
        auto efk = effect->AddComponent<EffectSystem>();
        efk->EffectID(0);
        efk->PlayOnStart(false);
        effect->Active(false);
        testObjects_.emplace(TestType::Effect, effect);
    }

    void SetupSprite()
    {
        auto spriteObj = GameObjectManager::Instantiate("Sprite");
        spriteObj->Transform()->Parent(GameObject()->Transform());
        spriteObj->Transform()->LocalPosition(Vector3::Up());
        spriteObj->Transform()->LocalScale(Vector3{ 0.1f, 0.1f, 0.0f });
        auto sprite = spriteObj->AddComponent<SpriteRenderer>();
        sprite->TextureID(0);
        spriteObj->Active(false);
        testObjects_.emplace(TestType::Sprite, spriteObj);
    }

    void SetupAudio()
    {
        auto audio = GameObjectManager::Instantiate("SE");
        audio->Transform()->Parent(GameObject()->Transform());
        auto se = audio->AddComponent<TestAudio>();
        audio->Active(false);
        testObjects_.emplace(TestType::Audio, audio);
    }

    void OnGUI() override
    {
        Component::OnGUI();
        if (GLGUI::ComboBox("Test Type", current_, combo_))
        {
            GameObject()->SendMsg(1, current_, GameObject());
            const auto& it = std::find(combo_.begin(), combo_.end(), current_);
            const auto idx = std::distance(combo_.begin(), it);
            currenType_ = static_cast<TestType>(idx);
        }
    }

private:
    std::vector<std::string> combo_{ "SkinnedMesh", "Effect", "Sprite", "Audio" };
    std::string current_{ "SkinnedMesh" };
    TestType currenType_{ TestType::SkinnedMesh };
    std::unordered_map<TestType, GameObjectPtr> testObjects_;
};

class TestLoadScene : public Scene
{
public:
    void Start() override
    {
        MeshManager::Load(0, "Assets/Model/Plane/Plane.globj");
        MeshManager::Load(1, "Assets/Model/Warrior/Warrior.globj");
        MeshManager::Load(2, "Assets/Model/Stage/Stage.globj");

        AnimationManager::Load(0, "Assets/Animation/Warrior@Idle1_0.glanim");
        AnimationManager::Load(1, "Assets/Animation/Warrior@nRun_F_1.glanim");

        SkyboxManager::Load(
            0,
            "Assets/Skybox/DayTop.png",
            "Assets/Skybox/DayBottom.png",
            "Assets/Skybox/DayLeft.png",
            "Assets/Skybox/DayRight.png",
            "Assets/Skybox/DayFront.png",
            "Assets/Skybox/DayBack.png"
        );

        TextureManager::Load(0, "Assets/Sprite/Ten_Icon.png");

        AudioManager::LoadVoice(0, "Assets/SE/correct_answer3.wav");

        EffectManager::Load(0, "Assets/Effect/Laser01.efk");

        SceneManager::LoadScene("TestScene");
    }

    void End() override
    {}
};

// テスト用シーンクラス
class TestScene : public Scene
{
public:
    void Start() override
    {

        // 平行光源設定
        auto light = GameObjectManager::Instantiate("Light")
            ->AddComponent<DirectionalLight>();
        light->Ambient(LIGHT_AMBIENT);
        light->Diffuse(LIGHT_DIFFUSE);
        light->Specular(LIGHT_SPECULAR);
        light->GameObject()->Transform()->EulerAngles(LIGHT_DIRECTION);

        // 音設定
        AudioManager::AddSoundGroup(0);
        AudioManager::SetSoundGroupVolume(0, 0.1f);

        // カメラ作成
        SkyboxManager::SetSkybox(0);
        auto camera = GameObjectManager::Instantiate("Camera");
        auto initPosition = Vector3{ 0.0f, 1.5f, -1.0f };
        camera->Transform()->Position(initPosition);
        camera->AddComponent<Camera>()->ClearFlags(CameraClearFlags::SkyBox);
        camera->AddComponent<AudioListener>();

        // 床
        auto Ground = GameObjectManager::Instantiate("Ground");
        Ground->AddComponent<MeshRenderer>()->MeshID(2);
        auto mc = Ground->AddComponent<MeshCollider>();
        mc->FlipNormals(true);
        mc->IsVisible(true);
        mc->MeshID(2);
        Ground->Transform()->Scale(Vector3{ 1, 1, 1 });


        auto tester = GameObjectManager::Instantiate("Tester");
        tester->AddComponent<Tester>();

        Debug::Log("Scene Loading...");
    }

    void End() override
    {
        Debug::Log("Scene End...");
    }
};

// ゲーム本体
class MyGame : public Game
{
    void Start() override
    {

        Debug::Log("GAME STARTTING");
        SceneManager::Register<TestLoadScene>();
        SceneManager::Register<TestScene>();
        Debug::Log("TestScene Load Start");
        SceneManager::LoadScene("TestLoadScene");
        Debug::Log("TestScene Load Complete");
    }

    void End() override
    {

    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Window::BorderlessWindow(true);
    Window::WindowSize(Vector2{ 2560.0f, 1440.0f });
    MyGame{}.Run();
    return 0;
}
