#include <Internal/EffekseerManager.h>
#pragma warning(push)
#pragma warning(disable: 6385)
#pragma warning(disable: 26495)
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma warning(pop)

#include <Internal/DX12/DirectX12.h>
#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Matrix4x4.h>
#include <GameTimer.h>
#include <StringUtility.h>
#include <FileUtility.h>
#include <Debugger.h>
#include <unordered_map>

namespace
{
    EffekseerRenderer::RendererRef s_efkRenderer{ nullptr };
    Effekseer::ManagerRef s_efkManager{ nullptr };
    Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> s_efkMemoryPool{ nullptr };
    Effekseer::RefPtr<EffekseerRenderer::CommandList> s_efkCommandList{ nullptr };
    std::unordered_map<unsigned int, Effekseer::EffectRef> s_effects{};
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();
}

namespace
{
    constexpr int MAX_SQAURE{ 8000 };
    constexpr float EFFECT_FPS{ 60.0f };
}

bool Glib::Internal::Effect::EffekseerManager::Initialize()
{
    // 描画デバイス作成
    auto graphicsDevice = EffekseerRendererDX12::CreateGraphicsDevice(
        s_dx12->Device().Get(),
        s_dx12->CommandQueue().Get(),
        s_dx12->BackBufferNum()
    );

    // レンダラーの作成
    auto format{ DXGI_FORMAT_R8G8B8A8_UNORM };
    s_efkRenderer = EffekseerRendererDX12::Create(
        graphicsDevice,
        &format,
        1,
        DXGI_FORMAT_D32_FLOAT,
        false,
        MAX_SQAURE
    );
    if (s_efkRenderer == nullptr) return false;

    // マネージャーの作成
    s_efkManager = Effekseer::Manager::Create(MAX_SQAURE);
    if (s_efkManager == nullptr) return false;

    // メモリープールの作成
    s_efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(s_efkRenderer->GetGraphicsDevice());
    if (s_efkMemoryPool == nullptr) return false;

    // コマンドリストの作成
    s_efkCommandList = EffekseerRenderer::CreateCommandList(s_efkRenderer->GetGraphicsDevice(), s_efkMemoryPool);
    if (s_efkCommandList == nullptr) return false;
    s_efkRenderer->SetCommandList(s_efkCommandList);

    // 描画モジュールの設定
    s_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
    s_efkManager->SetSpriteRenderer(s_efkRenderer->CreateSpriteRenderer());
    s_efkManager->SetRibbonRenderer(s_efkRenderer->CreateRibbonRenderer());
    s_efkManager->SetRingRenderer(s_efkRenderer->CreateRingRenderer());
    s_efkManager->SetTrackRenderer(s_efkRenderer->CreateTrackRenderer());
    s_efkManager->SetModelRenderer(s_efkRenderer->CreateModelRenderer());

    // テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
    s_efkManager->SetTextureLoader(s_efkRenderer->CreateTextureLoader());
    s_efkManager->SetModelLoader(s_efkRenderer->CreateModelLoader());
    s_efkManager->SetMaterialLoader(s_efkRenderer->CreateMaterialLoader());
    s_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

    return true;
}

void Glib::Internal::Effect::EffekseerManager::Update()
{
    // エフェクトの更新
    s_efkManager->Update(GameTimer::DeltaTime());
}

void Glib::Internal::Effect::EffekseerManager::Draw()
{
    for (const auto& camera : s_cameraManager->Cameras())
    {
        if (!camera->Active()) continue;

        camera->SetRenderTarget();

        Matrix4x4 proj;
        Matrix4x4 view;
        camera->ProjectionMatrix(&proj);
        camera->ViewMatrix(&view);

        // 描画開始処理
        s_efkMemoryPool->NewFrame();
        EffekseerRendererDX12::BeginCommandList(s_efkCommandList, s_dx12->CommandList().Get());
        s_efkRenderer->BeginRendering();

        // 行列の設定
        s_efkRenderer->SetProjectionMatrix(ToMatrix44(proj));
        s_efkRenderer->SetCameraMatrix(ToMatrix44(view));

        // 描画
        s_efkManager->Draw();

        // 描画終了処理
        s_efkRenderer->EndRendering();
        EffekseerRendererDX12::EndCommandList(s_efkCommandList);
    }
}

void Glib::Internal::Effect::EffekseerManager::Finalize()
{
    s_efkCommandList.Reset();
    s_efkMemoryPool.Reset();
    s_efkRenderer.Reset();
}

bool Glib::Internal::Effect::EffekseerManager::Load(unsigned int id, std::string_view path)
{
    if (!Glib::ExistsFile(path))
    {
        Debug::Error("Effect file does not exist.");
        return false;
    }

    auto effect = Effekseer::Effect::Create(s_efkManager, reinterpret_cast<char16_t*>(StringToWide(path).data()));
    if (effect == nullptr)
    {
        Debug::Error("Effect could not be loaded.");
        Debug::Error("Path: " + ToString(path));
        return false;
    }

    s_effects.emplace(id, effect);
    return true;
}

Effekseer::Matrix44 Glib::Internal::Effect::EffekseerManager::ToMatrix44(const Matrix4x4& matrix)
{
    Effekseer::Matrix44 mat;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            mat.Values[y][x] = matrix[y][x];
        }
    }
    return mat;
}
