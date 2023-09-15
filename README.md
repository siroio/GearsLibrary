# GearsLibrary

GearsLibraryは、コンポーネント指向を取り入れたゲーム制作用ライブラリです。

## 概要

GearsLibraryは、ゲーム開発者が効率的にゲームを制作するためのツールセットを提供します。  
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
    }

    void Update()
    {

    }

    void FixedUpdate()
    {

    }
};
```

```cpp
// テストシーンクラス
class TestScene : public Glib::Scene::Scene
{
public:
    // シーンロード時
    virtual void Start() override
    {
        Debug::Log("Scene Loaded...");
        auto go = GameObjectManager::Instantiate("TestObject");
        if (!go.expired()) Debug::Log("TestObject Created!");
        auto cmp = go->AddComponent<TestComponent>(); // コンポーネントの追加
        Debug::Log(Glib::ToString(cmp->Name()) + "ActiveStatus: " + std::to_string(cmp->Active()));
    }

    // シーン終了時
    virtual void End() override
    {
        Debug::Log("Scene Ended...");
    }
};
```

```cpp
// ゲーム本体
class MyGame : public Game
{
    // シーンの登録と推移
    virtual void Start() override
    {
        Debug::Log("GAME STARTTING");
        Glib::Scene::SceneManager::Register<TestScene>(); // シーンの登録
        Debug::Log("Scene: " + Glib::Scene::SceneManager::SceneName<TestScene>() + " Registered");
        Debug::Log("TestScene Load Start");
        Glib::Scene::SceneManager::LoadScene("TestScene"); // シーンのロード
        Debug::Log("TestScene Load Complete");
    }

    virtual void End() override
    {
        std::cout << "GAME END" << std::endl;
    }
};
```

```cpp
int main() // EntryPoint
{
    // ゲームクラスの実行
    MyGame{}.Run();
}
```
