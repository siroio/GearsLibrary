#include <Components/Camera.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/CameraManager.h>
#include <Internal/RenderingManager.h>
#include <GameObject.h>
#include <Window.h>
#include <Mathf.h>
#include <GLGUI.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_resource = Glib::Internal::Graphics::GraphicsResource::Instance();
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();
    auto s_renderingManager = Glib::Internal::Graphics::RenderingManager::Instance();

    /*　シャドウマップの大きさ　*/
    constexpr unsigned int SHADOW_MAP_SIZE = 2048;

    struct CameraConstant
    {
        Matrix4x4 View;
        Matrix4x4 Projection;
        Matrix4x4 LightVP;
    };

    std::vector<std::string> clearColors{ "SolidColor", "Skybox" };
    std::vector<std::string> projectionTypes{ "Perspective", "Orthographic" };
}

Glib::Camera::Camera()
{
    // バッファーを作成
    constantBuffer_.Create(sizeof(CameraConstant));
}

void Glib::Camera::Start()
{
    transform_ = GameObject()->Transform();
    InitializeRT();
    InitializeSM();
    s_cameraManager->AddCamera(shared_from_this());
}

void Glib::Camera::LateUpdate()
{
    if (transform_.expired()) return;

    CameraConstant buffer;
    ViewMatrix(buffer.View);
    ProjectionMatrix(buffer.Projection);
    buffer.LightVP = s_renderingManager->ComputeShadowMapViewMatrix(transform_->Position() + transform_->Forward());

    // バッファの更新
    constantBuffer_.Update(sizeof(buffer), &buffer);

    // レンダーターゲット、深度のクリア
    renderTarget_.AsRenderTarget();
    s_dx12->CommandList()->ClearRenderTargetView(renderTarget_.RTVHandle()->CPU(), backGroundColor_.Raw(), 0, nullptr);
    s_dx12->CommandList()->ClearDepthStencilView(renderTarget_.DSVHandle()->CPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    shadowMap_.AsRenderTarget();
    s_dx12->CommandList()->ClearRenderTargetView(shadowMap_.RTVHandle()->CPU(), Color::White().Raw(), 0, nullptr);
    s_dx12->CommandList()->ClearDepthStencilView(shadowMap_.DSVHandle()->CPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

const Color& Glib::Camera::BackGroundColor()
{
    return backGroundColor_;
}

void Glib::Camera::BackGroundColor(const Color& color)
{
    backGroundColor_ = color;
}

Glib::CameraClearFlags Glib::Camera::ClearFlags()
{
    return clearFlags_;
}

void Glib::Camera::ClearFlags(CameraClearFlags flags)
{
    clearFlags_ = flags;
}

Glib::ProjectionType Glib::Camera::Projection() const
{
    return projectionType_;
}

void Glib::Camera::Projection(ProjectionType type)
{
    projectionType_ = type;
}

float Glib::Camera::FieldOfView() const
{
    return fieldOfView_;
}

void Glib::Camera::FieldOfView(float fieldOfView)
{
    fieldOfView_ = fieldOfView;
}

float Glib::Camera::Near() const
{
    return near_;
}

void Glib::Camera::Near(float nearZ)
{
    near_ = nearZ;
}

float Glib::Camera::Far() const
{
    return far_;
}

void Glib::Camera::Far(float farZ)
{
    far_ = farZ;
}

void Glib::Camera::ViewPort(const Vector2& position, const Vector2& size)
{
    viewPortPosition_ = Vector2::Clamp(position, 0.0f, 1.0f);
    viewPortSize_ = Vector2::Clamp(size, 0.0f, 1.0f);
}

float Glib::Camera::OrthographicSize() const
{
    return orthographicSize_;
}

void Glib::Camera::OrthographicSize(float size)
{
    orthographicSize_ = Mathf::Min(0.0f, size * 2);
}

Vector3 Glib::Camera::WorldToScreenPoint(const Vector3& position)
{
    Matrix4x4 view;
    ViewMatrix(view);
    Matrix4x4 projection;
    ProjectionMatrix(projection);

    const auto& size = Window::WindowSize();
    const auto windowX = size.x * 0.5f;
    const auto windowY = size.y * 0.5f;
    Matrix4x4 screen{
        windowX, 0.0f, 0.0f, 0.0f,
        0.0f, -windowY, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        windowX, windowY, 0.0f, 1.0f
    };

    return position * view * projection * screen;
}

void Glib::Camera::InitializeRT()
{
    const Vector2& windowSize = Window::WindowSize();
    auto resDesc = s_dx12->BackBufferResourceDesc();
    auto srvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RES);

    // RenderTargetの作成
    renderTarget_.Create(
        static_cast<UINT>(windowSize.x),
        static_cast<UINT>(windowSize.y),
        backGroundColor_,
        resDesc.Format,
        DXGI_FORMAT_D32_FLOAT
    );

    // ShaderResourceView用のハンドルを取得
    srvHandle_ = srvPool->GetHandle();

    // ShaderResourceViewの作成
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = resDesc.Format;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    s_dx12->Device()->CreateShaderResourceView(
        renderTarget_.RenderTargetResource().Get(),
        &srvDesc,
        srvHandle_->CPU()
    );
}

void Glib::Camera::InitializeSM()
{
    const auto& windowSize = Window::WindowSize();
    const DXGI_FORMAT format{ DXGI_FORMAT_R32G32_FLOAT };
    shadowMap_.Create(
        SHADOW_MAP_SIZE,
        SHADOW_MAP_SIZE,
        Color::White(),
        format,
        DXGI_FORMAT_D32_FLOAT
    );

    shadowMapBlur.Initialize(shadowMap_.RenderTargetResource(), format);
}

void Glib::Camera::Draw()
{
    // RenderTargetをテクスチャとして利用できるように
    renderTarget_.AsTexture();

    // カメラ用パイプライン設定
    s_resource->SetPipelineState(Internal::Graphics::ID::CAMERA_PIPELINESTATE);
    // 頂点バッファの設定
    s_resource->SetVertexBuffer(Internal::Graphics::ID::CAMERA_VERTEX);

    const Vector2& size = Window::WindowSize();
    D3D12_VIEWPORT viewPort{};
    viewPort.TopLeftX = viewPortPosition_.x * size.x;
    viewPort.TopLeftY = viewPortPosition_.y * size.y;
    viewPort.Width = viewPortSize_.x * size.x;
    viewPort.Height = viewPortSize_.y * size.y;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;

    s_dx12->SetHeaps();
    s_dx12->CommandList()->RSSetViewports(1, &viewPort);
    s_dx12->CommandList()->SetGraphicsRootDescriptorTable(0, srvHandle_->GPU());
    s_dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    s_dx12->CommandList()->DrawInstanced(4, 1, 0, 0);
}

void Glib::Camera::ViewMatrix(Matrix4x4& mat) const
{
    transform_.expired() ?
        mat = Matrix4x4::Identity() :
        mat = Matrix4x4::LookAt(transform_->Position(), transform_->Position() + transform_->Forward(), transform_->Up());
}

void Glib::Camera::ProjectionMatrix(Matrix4x4& mat) const
{
    if (transform_.expired())
    {
        mat = Matrix4x4::Identity();
        return;
    }

    Vector2 viewPort = Vector2::Scale(viewPortSize_, Window::WindowSize());
    float aspect = viewPort.x / viewPort.y;

    switch (projectionType_)
    {
        case Glib::ProjectionType::Perspective:
            mat = Matrix4x4::Perspective(fieldOfView_, aspect, near_, far_);
            break;
        case Glib::ProjectionType::Orthographic:
            mat = Matrix4x4::Orthographic(aspect * orthographicSize_, orthographicSize_, near_, far_);
            break;
        default:
            mat = Matrix4x4::Identity();
            break;
    }
}

void Glib::Camera::SetRenderTarget()
{
    s_dx12->CommandList()->OMSetRenderTargets(
        1,
        &renderTarget_.RTVHandle()->CPU(),
        false,
        &renderTarget_.DSVHandle()->CPU()
    );

    const Vector2& windowSize = Window::WindowSize();
    CD3DX12_VIEWPORT viewPort{ 0.0f, 0.0f, windowSize.x, windowSize.y };
    CD3DX12_RECT rect{ 0, 0, static_cast<LONG>(windowSize.x), static_cast<LONG>(windowSize.y) };
    s_dx12->CommandList()->RSSetViewports(1, &viewPort);
    s_dx12->CommandList()->RSSetScissorRects(1, &rect);
}

void Glib::Camera::SetConstantBuffer(unsigned int rootParamIndex)
{
    constantBuffer_.SetBuffer(rootParamIndex);
}

void Glib::Camera::SetDepthStencil()
{
    CD3DX12_VIEWPORT viewPort{ 0.0f, 0.0f, static_cast<float>(SHADOW_MAP_SIZE), static_cast<float>(SHADOW_MAP_SIZE) };
    CD3DX12_RECT rect{ 0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE };

    s_dx12->CommandList()->OMSetRenderTargets(
        1, &shadowMap_.RTVHandle()->CPU(),
        false, &shadowMap_.DSVHandle()->CPU());

    s_dx12->CommandList()->RSSetViewports(1, &viewPort);
    s_dx12->CommandList()->RSSetScissorRects(1, &rect);
}

void Glib::Camera::SetShadowMap(unsigned int rootParamIndex)
{
    shadowMapBlur.SetTexture(rootParamIndex);
}

void Glib::Camera::ExecuteShadowBlur()
{
    shadowMap_.AsTexture();
    shadowMapBlur.Execute(1.0f);
}

void Glib::Camera::OnGUI()
{
    GLGUI::ColorBar3("Background", &backGroundColor_);

    std::string currentFlag = clearColors.at(static_cast<int>(clearFlags_));
    if (GLGUI::ComboBox("ClearFlag", currentFlag, clearColors))
    {
        auto it = std::find(clearColors.begin(), clearColors.end(), currentFlag);
        clearFlags_ = static_cast<CameraClearFlags>(std::distance(clearColors.begin(), it));
    }

    GLGUI::DragFloat("NearClip", &near_, 0.01f);
    GLGUI::DragFloat("FarClip", &far_, 0.01f, near_);

    std::string currentProj = projectionTypes.at(static_cast<int>(projectionType_));
    if (GLGUI::ComboBox("Projection", currentProj, projectionTypes))
    {
        auto it = std::find(projectionTypes.begin(), projectionTypes.end(), currentProj);
        projectionType_ = static_cast<ProjectionType>(std::distance(projectionTypes.begin(), it));
    }
    switch (projectionType_)
    {
        case ProjectionType::Perspective:
            GLGUI::DragFloat("Fov", &fieldOfView_, 1.0f, 0.0f, 179.999f);
            break;
        case ProjectionType::Orthographic:
            GLGUI::DragFloat("Size", &orthographicSize_);
            break;
    }

    GLGUI::DragVector2("Viewport Position", &viewPortPosition_, 0.01f, 0.0f, 1.0f);
    GLGUI::DragVector2("Viewport Size", &viewPortSize_, 0.01f, 0.0f, 1.0f);
}
