#include <DX12/Internal/DirectX12.h>
#include <DX12/Internal/DescriptorPool.h>
#include <DX12/Internal/RenderTarget.h>
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
    /* デバイス */
    ComPtr<ID3D12Device> s_device{ nullptr };

    /* ファクトリー */
    ComPtr<IDXGIFactory6> s_dxgiFactory{ nullptr };

    /* スワップチェーン */
    ComPtr<IDXGISwapChain4> s_swapChain{ nullptr };

    /* コマンドアロケーター */
    ComPtr<ID3D12CommandAllocator> s_cmdAllocator{ nullptr };

    /* コマンドリスト */
    ComPtr<ID3D12GraphicsCommandList> s_cmdList{ nullptr };

    /* コマンドキュー */
    ComPtr<ID3D12CommandQueue> s_cmdQueue{ nullptr };

    /* フェンス */
    ComPtr<ID3D12Fence> s_fence{ nullptr };

    /* フェンス値 */
    UINT64 s_fenceValue{ 0 };

    /* ディスクリプタプール */
    std::array<std::shared_ptr<Glib::Internal::Graphics::DescriptorPool>,
        static_cast<int>(Glib::Internal::Graphics::DirectX12::POOLTYPE::COUNT)> s_descriptors;

    /* バックバッファフレーム数 */
    constexpr unsigned int FRAME_COUNT{ 2 };

    /* バックバッファ */
    std::array<Glib::Internal::Graphics::RenderTarget, FRAME_COUNT> s_backBuffers;

    /* シザー矩形 */
    D3D12_RECT s_scissorRect{};

    /* ビューポート */
    D3D12_VIEWPORT s_viewPort{};

    /* 背景色 */
    Color s_backGroundColor = Color::Cyan();

    /* Windowインスタンス */
    Glib::Window& s_window = Glib::Window::Instance();
}

namespace
{
    /* ディスクリプタプールのサイズ */

    constexpr int RESOURCE_POOL_SIZE = 512;
    constexpr int SAMPLER_POOL_SIZE = 256;
    constexpr int RENDER_TARGET_VIEW_POOL_SIZE = 512;
    constexpr int DEPTH_STENCIL_VIEW_POOL_SIZE = 512;
}

bool Glib::Internal::Graphics::DirectX12::Initialize()
{
    if (!s_window.Initialize()) return false;
    EnableDebugLayer();
    // ファクトリーの作成
#if defined(DEBUG) || defined(_DEBUG)
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(s_dxgiFactory.ReleaseAndGetAddressOf())))) return false;
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
        if (!s_backBuffers[idx].Create(idx, s_swapChain.Get()))
            return false;
    }

    if (FAILED(s_device->CreateFence(s_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(s_fence.ReleaseAndGetAddressOf())))) return false;

#if defined(DEBUG) || defined(_DEBUG)
    const auto& windowSize = Window::WindowSize();
#else
    const auto& windowSize = Window::WindowDebugSize();
#endif

    // ビューポート設定
    s_viewPort.Width = windowSize.x;
    s_viewPort.Height = windowSize.y;
    s_viewPort.TopLeftX = 0;
    s_viewPort.TopLeftY = 0;
    s_viewPort.MaxDepth = 1.0f;
    s_viewPort.MinDepth = 0.0f;

    // シザー矩形設定
    s_scissorRect.left = 0;
    s_scissorRect.top = 0;
    s_scissorRect.right = s_scissorRect.left + static_cast<long>(windowSize.x);
    s_scissorRect.bottom = s_scissorRect.top + static_cast<long>(windowSize.y);

    return true;
}

void Glib::Internal::Graphics::DirectX12::BeginDraw()
{
    auto bbIdx = s_swapChain->GetCurrentBackBufferIndex();

    auto barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
        s_backBuffers[bbIdx].Resource().Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    s_cmdList->ResourceBarrier(1, &barrierDesc);

    auto& rtvH = s_backBuffers[bbIdx].Handle()->CPU();

    s_cmdList->OMSetRenderTargets(1, &rtvH, true, nullptr);
    s_cmdList->ClearRenderTargetView(rtvH, s_backGroundColor.rgba.data(), 0, nullptr);

    ID3D12DescriptorHeap* heaps[] = {
       s_descriptors[static_cast<int>(POOLTYPE::RES)]->GetHeap().Get()
    };
    s_cmdList->SetDescriptorHeaps(1, heaps);
}

void Glib::Internal::Graphics::DirectX12::EndDraw()
{
    auto bbIdx = s_swapChain->GetCurrentBackBufferIndex();
    auto barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
        s_backBuffers[bbIdx].Resource().Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    s_cmdList->ResourceBarrier(1, &barrierDesc);
    ExecuteCommandList();
    s_swapChain->Present(0, 0);
}

void Glib::Internal::Graphics::DirectX12::Finalize()
{
    WaitGPU();
    s_window.Finalize();
}

void Glib::Internal::Graphics::DirectX12::ExecuteCommandList()
{
    s_cmdList->Close();
    ComPtr<ID3D12CommandList> cmdLists[]{ s_cmdList.Get() };
    s_cmdQueue->ExecuteCommandLists(1, cmdLists->GetAddressOf());

    // GPU待機
    WaitGPU();

    s_cmdAllocator->Reset();
    s_cmdList->Reset(s_cmdAllocator.Get(), nullptr);
}

ComPtr<ID3D12Device> Glib::Internal::Graphics::DirectX12::Device() const
{
    return s_device;
}

ComPtr<ID3D12GraphicsCommandList> Glib::Internal::Graphics::DirectX12::CommandList() const
{
    return s_cmdList;
}

ComPtr<ID3D12CommandQueue> Glib::Internal::Graphics::DirectX12::CommandQueue() const
{
    return s_cmdQueue;
}

std::shared_ptr<Glib::Internal::Graphics::DescriptorPool> Glib::Internal::Graphics::DirectX12::DescriptorPool(POOLTYPE type) const
{
    return s_descriptors[static_cast<int>(type)];
}

D3D12_RESOURCE_DESC Glib::Internal::Graphics::DirectX12::BackBufferResourceDesc() const
{
    return s_backBuffers[0].ResourceDesc();
}

const Color& Glib::Internal::Graphics::DirectX12::BackGroundColor()
{
    return s_backGroundColor;
}

void Glib::Internal::Graphics::DirectX12::BackGroundColor(const Color& color)
{
    s_backGroundColor = color;
}

bool Glib::Internal::Graphics::DirectX12::InitDevice()
{
    ComPtr<IDXGIAdapter> adapter{ nullptr };
    ComPtr<IDXGIAdapter> nvidiaAdapter{ nullptr };
    ComPtr<IDXGIAdapter> maxVMAdapter{ nullptr };
    size_t videoMemroySize{};
    for (UINT i = 0; s_dxgiFactory->EnumAdapters(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
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

    const D3D_FEATURE_LEVEL levels[]{
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    for (auto&& level : levels)
    {
        auto result = SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(s_device.ReleaseAndGetAddressOf())));
        if (result == S_OK) break;
    }
    return s_device != nullptr;
}

bool Glib::Internal::Graphics::DirectX12::InitCommand()
{
    // アロケーターの作成
    auto result = s_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(s_cmdAllocator.ReleaseAndGetAddressOf()));
    if (FAILED(result)) return false;

    // リストの作成
    result = s_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, s_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(s_cmdList.ReleaseAndGetAddressOf()));
    if (FAILED(result)) return false;

    // キューの作成
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmdQueueDesc.NodeMask = 0;
    cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    result = s_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(s_cmdQueue.ReleaseAndGetAddressOf()));
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
    swapChainDesc.BufferCount = static_cast<UINT>(FRAME_COUNT);
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    return SUCCEEDED(s_dxgiFactory->CreateSwapChainForHwnd(
        s_cmdQueue.Get(),
        s_window.WindowHandle(),
        &swapChainDesc,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)s_swapChain.ReleaseAndGetAddressOf()
    ));
}

bool Glib::Internal::Graphics::DirectX12::CreateDescriptorPool()
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.NodeMask = 1;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.NumDescriptors = RESOURCE_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (!DescriptorPool::Create(s_device, &heapDesc, s_descriptors[static_cast<int>(POOLTYPE::RES)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    heapDesc.NumDescriptors = SAMPLER_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (!DescriptorPool::Create(s_device, &heapDesc, s_descriptors[static_cast<int>(POOLTYPE::SMP)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = RENDER_TARGET_VIEW_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (!DescriptorPool::Create(s_device, &heapDesc, s_descriptors[static_cast<int>(POOLTYPE::RTV)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    heapDesc.NumDescriptors = DEPTH_STENCIL_VIEW_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (!DescriptorPool::Create(s_device, &heapDesc, s_descriptors[static_cast<int>(POOLTYPE::DSV)])) return false;

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
    s_cmdQueue->Signal(s_fence.Get(), ++s_fenceValue);
    if (s_fence->GetCompletedValue() != s_fenceValue)
    {
        auto event = CreateEvent(nullptr, false, false, nullptr);
        s_fence->SetEventOnCompletion(s_fenceValue, event);
        if (event != 0)
        {
            WaitForSingleObjectEx(event, INFINITE, false);
            CloseHandle(event);
        }
    }
}
