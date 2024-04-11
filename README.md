# GearsLibrary

GearsLibraryは、コンポーネント指向を取り入れたゲーム制作用ライブラリです。

## 概要

GearsLibraryは、ゲーム開発者が効率的にゲームを制作するための機能を提供します。  
コンポーネント指向のアーキテクチャにより、ゲームオブジェクトやシステムを独立した部品に分割し、再利用性を高めつつ開発を行うことができます。

## 対応OS

- Windows

## 機能一覧

・テクスチャ

・スプライト描画

・メッシュ描画

・スカイボックス描画

・メッシュスキニング

・アニメーション

・エフェクト再生

・オーディオ再生

・コンポーネントクラス

・デバッグ用GUI

・メッシュ・アニメーションフォーマット

・MMDからのモデルコンバート

・ゲームシーン管理

・物理演算

・コライダー（ボックス、カプセル、メッシュ）

・数学系ライブラリ（Vector3、Quaternion、行列）

etc...


## 使用方法

```cpp
// テストコンポーネント
// Start Update FixedUpdate LateUpdate
class TestComponent : public Component
{
public:
    void Start()
    {
        Debug::Log("Enable TestComponent");

        // オブジェクトの座標を指定
        GameObject()->Transform()->Position(Vector3{ 0, 10, 0 });
    }
    
    void Update()
    {
        // 移動用処理
        auto& transform = GameObject()->Transform();
        Vector3 moveDir = Vector3::Zero();
        float speed = 4.0f;
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

        Vector3 translate = moveDir.Normalized() * speed * Glib::GameTimer::DeltaTime();
        transform->Position(transform->Position() + translate);
    }

    void FixedUpdate()
    {
        // 物理演算など
    }
};
```

```cpp
// テクスチャやその他アセットのロード
TextureManager::Load(0, "texture.png");
MeshManager::Load(0, "texture.globj");
AnimationManager::Load(0, "texture.glanim");
```

```cpp
// ゲーム本体
class MyGame : public Glib::Game
{
    void Start() override
    {
        Debug::Log("GAME STARTTING");
        // シーンを登録
        SceneManager::Register<TestScene>();

        // シーン名を取得
        Debug::Log("Scene: " + SceneManager::SceneName<TestScene>() + " Registered");
        Debug::Log("TestScene Load Start");

        // シーンを読み込み
        SceneManager::LoadScene("TestScene");
        Debug::Log("TestScene Load Complete");
    }

    void End() override
    {
        Debug::Log("GAME END");
    }
};
```

```cpp
int main()
{
    // ゲームクラスの実行
    MyGame{}.Run();
}
```
