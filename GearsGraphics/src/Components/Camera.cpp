#include <Components/Camera.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/GraphicsResource.h>
#include <Internal/DX12/GraphicsResourceID.h>
#include <Internal/CameraManager.h>
#include <GameObject.h>
#include <Window.h>
#include <Mathf.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
    auto s_resource = Glib::Internal::Graphics::GraphicsResource::Instance();
    auto s_cameraManager = Glib::Internal::Graphics::CameraManager::Instance();

    constexpr unsigned int SHADOW_MAP_SIZE = 2048;
}

struct CameraCBuffer
{
    Matrix4x4 View;
    Matrix4x4 Projection;
    Matrix4x4 Light;
};

Glib::Camera::Camera()
{
    // バッファーを作成
    cameraCBuffer_.Create(sizeof(CameraCBuffer));

}

void Glib::Camera::Start()
{
    transform_ = GameObject()->Transform();
    InitializeRT();
    InitializeSM();
    s_cameraManager->AddCamera(weak_from_this());
}

void Glib::Camera::LateUpdate()
{
    if (transform_.expired()) return;

    CameraCBuffer buffer;
    buffer.View = ViewMatrix();
    buffer.Projection = ProjectionMatrix();

    cameraCBuffer_.Update(sizeof(CameraCBuffer), &buffer);


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
    Matrix4x4 view = ViewMatrix();
    Matrix4x4 projection = ProjectionMatrix();

    const auto& size = Window::WindowSize();
    auto windowX = size.x / 2;
    auto windowY = size.y / 2;
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
    auto rtvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RTV);
    auto srvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RES);
    auto dsvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::DSV);

    // RenderTargetの作成
    resDesc.Width = static_cast<UINT>(windowSize.x);
    resDesc.Height = static_cast<UINT>(windowSize.y);
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT };
    auto clearValue = CD3DX12_CLEAR_VALUE{ DXGI_FORMAT_B8G8R8A8_UNORM, backGroundColor_.rgba.data() };
    s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValue,
        IID_PPV_ARGS(renderTarget_.ReleaseAndGetAddressOf())
    );

    // RenderTargetView用ハンドルを取得
    rtvHandle_ = rtvPool->GetHandle();

    // RenderTargetView作成
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    s_dx12->Device()->CreateRenderTargetView(
        renderTarget_.Get(),
        &rtvDesc,
        rtvHandle_->CPU()
    );

    // ShaderResourceView用のハンドルを取得
    srvHandle_ = srvPool->GetHandle();

    // ShaderResourceViewの作成
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    s_dx12->Device()->CreateShaderResourceView(
        renderTarget_.Get(),
        &srvDesc,
        srvHandle_->CPU()
    );

    // 深度バッファの作成
    D3D12_CLEAR_VALUE depthValue{};
    depthValue.Format = DXGI_FORMAT_D32_FLOAT;
    depthValue.DepthStencil.Depth = 1.0f;

    auto depthDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32_FLOAT, resDesc.Width, resDesc.Height);
    depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &depthDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthValue,
        IID_PPV_ARGS(depthStencil_.ReleaseAndGetAddressOf())
    );

    // DepthStencilView用のハンドル取得
    dsvHandle_ = dsvPool->GetHandle();

    // DepthStencilViewを作成
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    s_dx12->Device()->CreateDepthStencilView(
        depthStencil_.Get(),
        &dsvDesc,
        dsvHandle_->CPU()
    );
}

void Glib::Camera::InitializeSM()
{
    const auto& windowSize = Window::WindowSize();
    shadowMap_.Create(
        SHADOW_MAP_SIZE,
        SHADOW_MAP_SIZE,
        Color::White(),
        DXGI_FORMAT_R32G32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}

void Glib::Camera::Draw()
{
    //TODO: barrier

    // カメラ用パイプライン設定
    s_resource->SetPipelineState(Internal::Graphics::ID::CAMERA_VERTEX);
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

    s_dx12->CommandList()->RSSetViewports(1, &viewPort);

}

Matrix4x4 Glib::Camera::ViewMatrix() const
{
    return transform_.expired() ?
        Matrix4x4::Identity() :
        Matrix4x4::LookAt(transform_->Position(), transform_->Position() + transform_->Forward(), transform_->Up());
}

Matrix4x4 Glib::Camera::ProjectionMatrix() const
{
    if (transform_.expired()) return Matrix4x4::Identity();

    auto viewPort = Vector2::Scale(viewPortSize_, Window::WindowSize());
    float aspect = viewPort.x / viewPort.y;

    switch (projectionType_)
    {
        case Glib::ProjectionType::Perspective:
            return Matrix4x4::Perspective(fieldOfView_, aspect, near_, far_);
        case Glib::ProjectionType::Orthographic:
            return Matrix4x4::Orthographic(aspect * orthographicSize_, orthographicSize_, near_, far_);
        default:
            return Matrix4x4::Identity();
    }
}

void Glib::Camera::SetConstantBuffer(unsigned int rootParamIndex)
{
    cameraCBuffer_.BindPipeline(rootParamIndex);
}

void Glib::Camera::SetDepthStencil()
{

}

void Glib::Camera::SetShadowMap(unsigned int rootParamIndex)
{

}

void Glib::Camera::ShadowBulr()
{

}
