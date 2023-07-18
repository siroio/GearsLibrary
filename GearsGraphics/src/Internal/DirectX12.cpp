#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <Internal/DirectX12.h>
#include <Internal/DescriptorPool.h>
#include <Internal/RenderTarget.h>
#include <Window.h>
#include <Color.h>
#include <Vector2.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <string>
#include <array>
#include <vector>
#include <memory>

namespace
{
    /* 背景色 */
    Color backGroundColor_ = Color::Cyan();

    /* バックバッファフレーム数 */
    constexpr unsigned int FRAME_COUNT{ 2 };
}

namespace
{
    /* デバイス */
    ComPtr<ID3D12Device> device_{ nullptr };

    /* ファクトリー */
    ComPtr<IDXGIFactory6> dxgiFactory_{ nullptr };

    /* スワップチェーン */
    ComPtr<IDXGISwapChain4> swapChain_{ nullptr };

    /* コマンドアロケーター */
    ComPtr<ID3D12CommandAllocator> cmdAllocator_{ nullptr };

    /* コマンドリスト */
    ComPtr<ID3D12GraphicsCommandList> cmdList_{ nullptr };

    /* コマンドキュー */
    ComPtr<ID3D12CommandQueue> cmdQueue_{ nullptr };

    /* フェンス */
    ComPtr<ID3D12Fence> fence_{ nullptr };

    /* フェンス値 */
    UINT64 fenceValue_{ 0 };

    /* バックバッファ */
    Glib::Internal::Graphics::RenderTarget backBuffers_[FRAME_COUNT];

    /* ディスクリプタプール */
    std::array<std::shared_ptr<Glib::Internal::Graphics::DescriptorPool>,
        static_cast<int>(Glib::Internal::Graphics::DirectX12::POOLTYPE::COUNT)> descriptors_;

    /* シザー矩形 */
    D3D12_RECT scissorRect{};

    /* ビューポート */
    D3D12_VIEWPORT viewPort{};

    /* Windowインスタンス */
    Glib::Window& window_ = Glib::Window::Instance();
}

bool Glib::Internal::Graphics::DirectX12::Initialize()
{
    if (!window_.Initialize()) return false;
    EnableDebugLayer();
    // ファクトリーの作成
#if defined(DEBUG) || defined(_DEBUG)
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(dxgiFactory_.ReleaseAndGetAddressOf())))) return false;
#else
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory_.ReleaseAndGetAddressOf())))) return false;
#endif
    if (!InitDevice()) return false;
    if (!InitCommand()) return false;
    if (!CreateSwapChain()) return false;
    if (!CreateDescriptorPool()) return false;

    // バックバッファの作成
    for (auto idx = 0; idx < FRAME_COUNT; idx++)
    {
        if (!backBuffers_[idx].Create(idx, swapChain_.Get()))
            return false;
    }

    if (FAILED(device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence_.ReleaseAndGetAddressOf())))) return false;

#if defined(DEBUG) || defined(_DEBUG)
    const auto& windowSize = Window::WindowSize();
#else
    const auto& windowSize = Window::WindowDebugSize();
#endif

    // ビューポート設定
    viewPort.Width = windowSize.x;
    viewPort.Height = windowSize.y;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.MaxDepth = 1.0f;
    viewPort.MinDepth = 0.0f;

    // シザー矩形設定
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = scissorRect.left + static_cast<long>(windowSize.x);
    scissorRect.bottom = scissorRect.top + static_cast<long>(windowSize.y);

    return true;
}

void Glib::Internal::Graphics::DirectX12::BeginDraw()
{
    auto bbIdx = swapChain_->GetCurrentBackBufferIndex();

    auto barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
        backBuffers_[bbIdx].Resource().Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    cmdList_->ResourceBarrier(1, &barrierDesc);

    auto& rtvH = backBuffers_[bbIdx].Handle()->CPU();

    cmdList_->OMSetRenderTargets(1, &rtvH, true, nullptr);
    float color[]{ backGroundColor_[0], backGroundColor_[1], backGroundColor_[2], backGroundColor_[3] };
    cmdList_->ClearRenderTargetView(rtvH, color, 0, nullptr);
}

void Glib::Internal::Graphics::DirectX12::EndDraw()
{
    auto bbIdx = swapChain_->GetCurrentBackBufferIndex();
    auto barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
        backBuffers_[bbIdx].Resource().Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    cmdList_->ResourceBarrier(1, &barrierDesc);
    ExecuteCommandList();
    swapChain_->Present(0, 0);
}

void Glib::Internal::Graphics::DirectX12::Finalize()
{
    WaitGPU();
    window_.Finalize();
}

void Glib::Internal::Graphics::DirectX12::ExecuteCommandList()
{
    cmdList_->Close();
    ID3D12CommandList* cmdLists[]{ cmdList_.Get() };
    cmdQueue_->ExecuteCommandLists(1, cmdLists);
    WaitGPU();

    cmdAllocator_->Reset();
    cmdList_->Reset(cmdAllocator_.Get(), nullptr);
}

ComPtr<ID3D12Device> Glib::Internal::Graphics::DirectX12::Device() const
{
    return device_;
}

ComPtr<ID3D12GraphicsCommandList> Glib::Internal::Graphics::DirectX12::CommandList() const
{
    return cmdList_;
}

ComPtr<ID3D12CommandQueue> Glib::Internal::Graphics::DirectX12::CommandQueue() const
{
    return cmdQueue_;
}

std::shared_ptr<Glib::Internal::Graphics::DescriptorPool> Glib::Internal::Graphics::DirectX12::DescriptorPool(POOLTYPE type) const
{
    return descriptors_[static_cast<int>(type)];
}

D3D12_RESOURCE_DESC Glib::Internal::Graphics::DirectX12::BackBufferResourceDesc() const
{
    return backBuffers_[0].ResourceDesc();
}

const Color& Glib::Internal::Graphics::DirectX12::BackGroundColor()
{
    return backGroundColor_;
}

void Glib::Internal::Graphics::DirectX12::BackGroundColor(const Color& color)
{
    backGroundColor_ = color;
}

bool Glib::Internal::Graphics::DirectX12::InitDevice()
{
    D3D_FEATURE_LEVEL levels[]{
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    ComPtr<IDXGIAdapter> adapter{ nullptr };
    ComPtr<IDXGIAdapter> nvidiaAdapter{ nullptr };
    ComPtr<IDXGIAdapter> maxVMAdapter{ nullptr };
    size_t videoMemroySize{};
    for (UINT i = 0; dxgiFactory_->EnumAdapters(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC adptDesc{};
        adapter->GetDesc(&adptDesc);
        std::wstring strDesc = adptDesc.Description;
        if (strDesc.find(L"NVIDIA") != std::string::npos)
        {
            nvidiaAdapter = adapter;
        }
        else if (adptDesc.DedicatedVideoMemory > videoMemroySize)
        {
            maxVMAdapter = adapter;
            videoMemroySize = adptDesc.DedicatedVideoMemory;
        }
    }

    adapter = nvidiaAdapter != nullptr ? nvidiaAdapter : maxVMAdapter;

    for (auto&& level : levels)
    {
        auto result = SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(device_.ReleaseAndGetAddressOf())));
        if (result == S_OK) break;
    }

    return device_ != nullptr;
}

bool Glib::Internal::Graphics::DirectX12::InitCommand()
{
    // アロケーターの作成
    auto result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator_.ReleaseAndGetAddressOf()));
    if (FAILED(result)) return false;

    // リストの作成
    result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(cmdList_.ReleaseAndGetAddressOf()));
    if (FAILED(result)) return false;

    // キューの作成
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmdQueueDesc.NodeMask = 0;
    cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(cmdQueue_.ReleaseAndGetAddressOf()));
    return !FAILED(result);
}

bool Glib::Internal::Graphics::DirectX12::CreateSwapChain()
{
#if defined(DEBUG) || defined(_DEBUG)
    const auto& windowSize = Window::WindowSize();
#else
    const auto& windowSize = Window::WindowDebugSize();
#endif

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = static_cast<UINT>(windowSize.x);
    swapChainDesc.Height = static_cast<UINT>(windowSize.y);
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    return SUCCEEDED(dxgiFactory_->CreateSwapChainForHwnd(
        cmdQueue_.Get(),
        window_.WindowHandle(),
        &swapChainDesc,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)swapChain_.ReleaseAndGetAddressOf()
    ));
}

bool Glib::Internal::Graphics::DirectX12::CreateDescriptorPool()
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.NodeMask = 1;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.NumDescriptors = 512;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (!DescriptorPool::Create(device_, &heapDesc, descriptors_[static_cast<int>(POOLTYPE::RES)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    heapDesc.NumDescriptors = 256;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (!DescriptorPool::Create(device_, &heapDesc, descriptors_[static_cast<int>(POOLTYPE::SMP)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = 512;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (!DescriptorPool::Create(device_, &heapDesc, descriptors_[static_cast<int>(POOLTYPE::RTV)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    heapDesc.NumDescriptors = 512;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (!DescriptorPool::Create(device_, &heapDesc, descriptors_[static_cast<int>(POOLTYPE::DSV)])) return false;

    return true;
}

void Glib::Internal::Graphics::DirectX12::EnableDebugLayer()
{
#if defined(DEBUG) || defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController{ nullptr };
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()))))
    {
        debugController->EnableDebugLayer();
    }
#endif
}

void Glib::Internal::Graphics::DirectX12::WaitGPU()
{
    cmdQueue_->Signal(fence_.Get(), ++fenceValue_);
    if (fence_->GetCompletedValue() != fenceValue_)
    {
        auto event = CreateEvent(nullptr, false, false, nullptr);
        fence_->SetEventOnCompletion(fenceValue_, event);
        if (event != 0)
        {
            WaitForSingleObjectEx(event, INFINITE, false);
            CloseHandle(event);
        }
    }
}
