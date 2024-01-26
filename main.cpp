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
#include <filesystem>

using namespace Glib;

namespace
{
    const Vector3 LIGHT_DIRECTION{ 30.0f, 40.0f, 0.0f };
    const Color LIGHT_AMBIENT{ 0.7f, 0.7f, 0.7f, 1.0f };
    const Color LIGHT_DIFFUSE{ 0.7f, 0.7f, 0.7f, 1.0f };
}

// テスト用コンポーネント
class TestMover : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable " + nameof(*this));
    }

    void Update()
    {
        auto& transform = GameObject()->Transform();
        Vector3 velocity;
        Vector3 rotation;
        float speed = 10;
        float rotSpeed = 270;

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
            rotation.y -= rotSpeed * GameTimer::DeltaTime();
        }
        if (InputSystem::GetKey(KeyCode::D))
        {
            rotation.y += rotSpeed * GameTimer::DeltaTime();
        }

        auto lstick = InputSystem::GetLeftStick();
        auto rstick = InputSystem::GetRightStick();
        if (lstick.SqrMagnitude() > 0.1f || rstick.SqrMagnitude() > 0.1f)
        {
            lstick *= speed * GameTimer::DeltaTime();
            rstick.y *= speed * GameTimer::DeltaTime();
            velocity = Vector3{ lstick.x, lstick.y, rstick.y };
            rotation.y = rstick.x * rotSpeed * GameTimer::DeltaTime();
        }

        transform->Position(transform->Position() + velocity);
        transform->Rotate(rotation);
    }
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
        }
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

        int meshID{ 0 }, animID{ 0 }, audiID{ 0 }, texID{ 0 }, effectID{ 0 };
        for (const auto& entry : std::filesystem::recursive_directory_iterator("Assets"))
        {
            if (!entry.is_regular_file()) continue;
            auto ext = entry.path().extension().string();
            if (ext.ends_with("globj"))
            {
                isloaded = MeshManager::Instance().Load(meshID, entry.path().string());
                if (!isloaded)
                {
                    Debug::Error(entry.path().string() + "のロードに失敗しました。");
                    continue;
                }
                meshID++;
            }
            if (ext.ends_with("glanim"))
            {
                isloaded = AnimationManager::Instance().Load(animID, entry.path().string());
                if (!isloaded)
                {
                    Debug::Error(entry.path().string() + "のロードに失敗しました。");
                    continue;
                }
                animID++;
            }
            if (ext.ends_with("wav"))
            {
                isloaded = AudioManager::Instance()->LoadVoice(audiID, entry.path().string());
                if (!isloaded)
                {
                    Debug::Error(entry.path().string() + "のロードに失敗しました。");
                    continue;
                }
                audiID++;
            }
            if (ext.ends_with("efk"))
            {
                isloaded = EffectManager::Load(effectID, entry.path().string());
                if (!isloaded)
                {
                    Debug::Error(entry.path().string() + "のロードに失敗しました。");
                    continue;
                }
                effectID++;
            }
        }
        AudioManager::Instance()->AddSoundGroup(0);
        AudioManager::Instance()->SetSoundGroupVolume(0, 0.1f);
        for (const auto& entry : std::filesystem::recursive_directory_iterator("Assets/Sprite"))
        {
            if (!entry.is_regular_file()) continue;
            auto ext = entry.path().extension().string();
            if (ext.ends_with("png"))
            {
                isloaded = TextureManager::Instance().Load(texID, entry.path().string());
                if (!isloaded)
                {
                    Debug::Error(entry.path().string() + "のロードに失敗しました。");
                    continue;
                }
            }
        }

        // 平行光源設定
        auto light = GameObjectManager::Instantiate("Light")
            ->AddComponent<DirectionalLight>();
        light->Ambient(LIGHT_AMBIENT);
        light->Diffuse(LIGHT_DIFFUSE);
        light->GameObject()->Transform()->EulerAngles(LIGHT_DIRECTION);

        // カメラ作成
        SkyboxManager::Instance()->SetSkybox(0);
        auto camera = GameObjectManager::Instantiate("Camera");
        auto initPosition = Vector3{ 0.0f, 10.0f, -20.0f };
        camera->Transform()->Position(initPosition);
        camera->AddComponent<Camera>()->ClearFlags(CameraClearFlags::SolidColor);
        camera->AddComponent<AudioListener>();

        // オブジェクト生成
        auto mesh = GameObjectManager::Instantiate("Mesh");
        auto renderer = mesh->AddComponent<SkinnedMeshRenderer>();
        auto animator = mesh->AddComponent<Animator>();
        mesh->AddComponent<TestMover>();
        renderer->MeshID(0);
        animator->AnimationID(0);
        animator->Loop(true);
        mesh->AddComponent<TestAudio>();

        auto effect = GameObjectManager::Instantiate("Effect");
        auto efk = effect->AddComponent<EffectSystem>();
        efk->EffectID(0);
        efk->Play();

        //auto spriteObj = GameObjectManager::Instantiate("Sprite");
        //auto sprite = spriteObj->AddComponent<SpriteRenderer>();
        //sprite->TextureID(0);

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
        SceneManager::Register<TestScene>();
        Debug::Log("Scene: " + SceneManager::SceneName<TestScene>() + " Registered");
        Debug::Log("TestScene Load Start");
        SceneManager::LoadScene("TestScene");
        Debug::Log("TestScene Load Complete");
    }

    void End() override
    {
        Debug::Log("GAME END");
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    MyGame{}.Run();
}
