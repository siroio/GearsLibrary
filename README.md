# GearsLibrary

GearsLibraryは、コンポーネント指向を取り入れたゲーム制作用ライブラリです。

## 概要

GearsLibraryは、ゲーム開発者が効率的にゲームを制作するための機能を提供します。  
コンポーネント指向のアーキテクチャにより、ゲームオブジェクトやシステムを独立した部品に分割し、再利用性を高めつつ開発を行うことができます。

## 対応OS

- Windows

## ビルド方法

プロジェクトをビルドするには、以下の手順に従ってください。

1. CMakeをインストールしていない場合は、[公式ウェブサイト](https://cmake.org/download/)からダウンロードしてインストールしてください。

2. リポジトリをクローンするか、ZIPファイルとしてダウンロードして展開します。

3. ターミナルまたはコマンドプロンプトを開き、プロジェクトのディレクトリに移動します。

4. `Build.bat` スクリプトを実行します。

```bash
> Build.bat
```

## 機能一覧

* 編集中

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
        GameObject()->Transform()->Position(Vector3{ 1280, 720, 0 });
    }
    
    void Update()
    {
        // 移動用処理
        auto& transform = GameObject()->Transform();
        Vector3 velocity;
        float speed = 500;
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

        transform->Position(transform->Position() + velocity);
    }

    void FixedUpdate()
    {
        // 物理演算など
    }
};
```

```cpp
// テスト用シーンクラス
class TestScene : public Glib::Scene
{
public:
    // シーン読み込み
    void Start() override
    {
        // テクスチャを読み込み
        auto& tex = TextureManager::Instance();
        tex.Load(0, "texture.png");
        Debug::Log("Scene Loaded...");

        // キャンバスコンポーネントを持ったオブジェクトを生成
        auto canvas = GameObjectManager::Instantiate("Canvas");
        canvas->AddComponent<Canvas>();
        
        // Imageコンポーネントを持ったオブジェクト生成
        auto img = GameObjectManager::Instantiate("Img");
        img->Transform()->Parent(canvas->Transform());// キャンバスの子オブジェクトに
        auto imgComp = img->AddComponent<Image>();
        img->AddComponent<TestComponent>();
        
        // テクスチャと色を指定
        imgComp->TextureID(0);
        imgComp->Color(Color{ 1.0f, 1.0f, 1.0f, 1.0f });
    }

    void End() override
    {
        Debug::Log("Scene Ended...");
    }
};
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
