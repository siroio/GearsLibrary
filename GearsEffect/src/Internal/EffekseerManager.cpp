﻿#include <Internal/EffekseerManager.h>
#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 6385)
#pragma warning(disable: 26495)
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma warning(pop)

#include <Internal/DX12/DirectX12.h>
#include <Internal/CameraManager.h>
#include <Internal/CameraBase.h>
#include <Matrix4x4.h>
#include <Vector3.h>
#include <Color.h>
#include <Random.h>
#include <GameTimer.h>
#include <StringUtility.h>
#include <FileUtility.h>
#include <Debugger.h>
#include <unordered_map>
#include <deque>

namespace
{
    EffekseerRenderer::RendererRef s_efkRenderer{ nullptr };
    Effekseer::ManagerRef s_efkManager{ nullptr };
    std::deque<Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool>> s_efkMemoryPools{ nullptr };
    std::deque<Effekseer::RefPtr<EffekseerRenderer::CommandList>> s_efkCommandLists{ nullptr };
    std::unordered_map<unsigned int, Effekseer::EffectRef> s_effects{};

    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();
}

namespace
{
    // 最大描画数
    constexpr int MAX_SQAURE{ 8000 };

    // エフェクトのフレーム速度
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

    // per frame

    s_efkMemoryPools.resize(s_dx12->BackBufferNum());
    s_efkCommandLists.resize(s_dx12->BackBufferNum());
    for (int i = 0; i < s_efkMemoryPools.size(); i++)
    {
        // メモリープールの作成
        auto memPool = EffekseerRenderer::CreateSingleFrameMemoryPool(s_efkRenderer->GetGraphicsDevice());
        if (memPool == nullptr) return false;
        s_efkMemoryPools.at(i) = memPool;

        // コマンドリストの作成
        auto cmdList = EffekseerRenderer::CreateCommandList(s_efkRenderer->GetGraphicsDevice(), memPool);
        if (cmdList == nullptr) return false;
        s_efkCommandLists.at(i) = cmdList;
    }

    // 現在のコマンドリストを設定
    s_efkRenderer->SetCommandList(s_efkCommandLists.at(s_dx12->CurrentBackBufferIndex()));

    // 描画モジュールの設定
    s_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
    s_efkManager->SetSpriteRenderer(s_efkRenderer->CreateSpriteRenderer());
    s_efkManager->SetRibbonRenderer(s_efkRenderer->CreateRibbonRenderer());
    s_efkManager->SetRingRenderer(s_efkRenderer->CreateRingRenderer());
    s_efkManager->SetTrackRenderer(s_efkRenderer->CreateTrackRenderer());
    s_efkManager->SetModelRenderer(s_efkRenderer->CreateModelRenderer());
    s_efkManager->SetRandFunc(Random::Next);

    // テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
    s_efkManager->SetTextureLoader(s_efkRenderer->CreateTextureLoader());
    s_efkManager->SetModelLoader(s_efkRenderer->CreateModelLoader());
    s_efkManager->SetMaterialLoader(s_efkRenderer->CreateMaterialLoader());
    s_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

    return true;
}

void Glib::Internal::Effect::EffekseerManager::Finalize()
{
    s_effects.clear();
    s_efkManager.Reset();
    s_efkCommandLists.clear();
    s_efkMemoryPools.clear();
    s_efkRenderer.Reset();
}

void Glib::Internal::Effect::EffekseerManager::Update()
{
    // エフェクトの更新
    s_efkManager->Update(GameTimer::DeltaTime() * EFFECT_FPS);
}

void Glib::Internal::Effect::EffekseerManager::Draw()
{
    for (const auto& camera : s_cameraManager->Cameras())
    {
        if (!camera->Active()) continue;

        // 描画先指定
        camera->SetRenderTarget();

        // 変換行列の取得
        Matrix4x4 view, proj;
        camera->ViewMatrix(view);
        camera->ProjectionMatrix(proj);

        // 行列の設定
        s_efkRenderer->SetCameraMatrix(ToMatrix44(view));
        s_efkRenderer->SetProjectionMatrix(ToMatrix44(proj));

        // コマンドリストの取得
        const int bbIdx = s_dx12->CurrentBackBufferIndex();
        const auto& cmdList = s_efkCommandLists.at(bbIdx);

        // 描画開始処理
        s_efkMemoryPools.at(bbIdx)->NewFrame();
        EffekseerRendererDX12::BeginCommandList(cmdList, s_dx12->CommandList().Get());
        s_efkRenderer->SetCommandList(cmdList);
        s_efkRenderer->BeginRendering();

        // 描画
        s_efkManager->Draw();

        // 描画終了処理
        s_efkRenderer->EndRendering();
        s_efkRenderer->SetCommandList(nullptr);
        EffekseerRendererDX12::EndCommandList(cmdList);
    }
}

bool Glib::Internal::Effect::EffekseerManager::Load(unsigned int id, std::string_view path)
{
    // エフェクトファイルが存在しているかチェック
    if (!Glib::ExistsFile(path))
    {
        Debug::Error("Effect file does not exist.");
        return false;
    }

    auto effect = Effekseer::Effect::Create(s_efkManager, reinterpret_cast<const EFK_CHAR*>(StringToWide(path).data()));

    // エフェクトが作成できたかチェック
    if (effect == nullptr)
    {
        Debug::Error("Effect could not be loaded.");
        Debug::Error("Path: " + ToString(path));
        return false;
    }

    s_effects.emplace(id, effect);
    return true;
}

bool Glib::Internal::Effect::EffekseerManager::Contains(unsigned int id)
{
    return s_effects.contains(id);
}

bool Glib::Internal::Effect::EffekseerManager::Exists(EffectHandle handle)
{
    return s_efkManager->Exists(handle);
}

EffectHandle Glib::Internal::Effect::EffekseerManager::Play(unsigned int id, const Vector3& position)
{
    auto effect = s_effects.find(id);
    if (effect == s_effects.end())
    {
        Debug::Error("Invalid EffectID : " + std::to_string(id));
        return EFFEKSEER_INVALID_HANDLE;
    }

    auto handle = s_efkManager->Play(effect->second, { position.x, position.y, position.z });
    return handle;
}

void Glib::Internal::Effect::EffekseerManager::Pause(EffectHandle handle)
{
    s_efkManager->SetPaused(handle, true);
}

void Glib::Internal::Effect::EffekseerManager::UnPause(EffectHandle handle)
{
    s_efkManager->SetPaused(handle, false);
}

void Glib::Internal::Effect::EffekseerManager::Stop(EffectHandle handle)
{
    s_efkManager->StopEffect(handle);
}

void Glib::Internal::Effect::EffekseerManager::SetSpeed(EffectHandle handle, float speed)
{
    s_efkManager->SetSpeed(handle, speed);
}

void Glib::Internal::Effect::EffekseerManager::SetColor(EffectHandle handle, const Color& color)
{
    Effekseer::Color destColor{};
    destColor.R = static_cast<uint8_t>(color.r * 255.0f);
    destColor.G = static_cast<uint8_t>(color.g * 255.0f);
    destColor.B = static_cast<uint8_t>(color.b * 255.0f);
    destColor.A = static_cast<uint8_t>(color.a * 255.0f);
    s_efkManager->SetAllColor(handle, destColor);
}

void Glib::Internal::Effect::EffekseerManager::SetTargetPosition(EffectHandle handle, const Vector3& position)
{
    s_efkManager->SetTargetLocation(handle, { position.x, position.y, position.z });
}

void Glib::Internal::Effect::EffekseerManager::SetMatrix(EffectHandle handle, const Matrix4x4& matrix)
{
    s_efkManager->SetMatrix(handle, ToMatrix43(matrix));
}

Effekseer::Matrix44 Glib::Internal::Effect::EffekseerManager::ToMatrix44(const Matrix4x4& matrix)
{
    Effekseer::Matrix44 mat{};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat.Values[i][j] = matrix[i][j];
        }
    }
    return mat;
}

Effekseer::Matrix43 Glib::Internal::Effect::EffekseerManager::ToMatrix43(const Matrix4x4& matrix)
{
    Effekseer::Matrix43 mat{};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat.Value[i][j] = matrix[i][j];
        }
    }
    return mat;
}
