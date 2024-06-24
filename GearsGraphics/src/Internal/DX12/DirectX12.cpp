#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/CommandAllocator.h>
#include <Internal/DX12/CommandList.h>
#include <Internal/DX12/CommandQueue.h>
#include <Internal/DX12/Fence.h>
#include <Internal/DX12/DynamicConstantBuffer.h>
#include <RenderTarget.h>
#include <Window.h>
#include <Color.h>
#include <Vector2.h>
#include <dxgi1_6.h>
#include <string>
#include <array>
#include <vector>
#include <memory>

using namespace Glib::Internal::Graphics;

namespace
{
    /* フレーム数 */
    constexpr unsigned int FRAME_COUNT{ 2 };

    /* デバイス */
    ComPtr<ID3D12Device> s_device{ nullptr };

    /* ファクトリー */
    ComPtr<IDXGIFactory6> s_dxgiFactory{ nullptr };

    /* スワップチェーン */
    ComPtr<IDXGISwapChain4> s_swapChain{ nullptr };

    /* コマンド系統 */

    std::array<std::shared_ptr<CommandAllocator>, FRAME_COUNT> s_cmdAllocator{};
    std::array<std::shared_ptr<CommandList>, FRAME_COUNT> s_cmdList{};
    std::shared_ptr<CommandQueue> s_cmdQueue{};

    /* フェンス */
    std::array<Fence, FRAME_COUNT> s_fence{};

    /*　動的定数バッファのサイズ */
    constexpr uint32_t CONSTANT_BUFFER_SIZE{ 256 * 2000 };

    /* 動的定数バッファ */
    std::array<std::shared_ptr<DynamicConstantBuffer>, FRAME_COUNT> s_constatnBuffers;

    /* ディスクリプタプール */
    std::array<std::shared_ptr<DescriptorPool>,
        static_cast<int>(DirectX12::PoolType::COUNT)> s_descriptors;

    /* 描画フレーム */

    int s_prevFrame{ -1 };
    int s_currentFrame{ 0 };

    /* バックバッファ */
    std::unique_ptr<Glib::Graphics::RenderTarget[]> s_backBuffers;

    /* シザー矩形 */
    D3D12_RECT s_scissorRect{};

    /* ビューポート */
    D3D12_VIEWPORT s_viewPort{};

    /* 背景色 */
    Color s_backGroundColor = Color::Black();

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

bool DirectX12::Initialize()
{
    if (!s_window.Initialize()) return false;
    EnableDebugLayer();
    // ファクトリーの作成
#if defined(DEBUG) || defined(_DEBUG)
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(s_dxgiFactory.ReleaseAndGetAddressOf())))) return false;
#else
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(s_dxgiFactory.ReleaseAndGetAddressOf())))) return false;
#endif
    if (!InitDevice()) return false;
    if (!InitCommand()) return false;
    if (!CreateSwapChain()) return false;
    if (!CreateDescriptorPool()) return false;

    // 遅延フレーム数を1に設定
    s_swapChain->SetMaximumFrameLatency(1);

    // １フレームに必要なものを作成
    s_backBuffers = std::make_unique<Glib::Graphics::RenderTarget[]>(FRAME_COUNT);
    for (auto idx = 0; idx < FRAME_COUNT; idx++)
    {
        if (!s_backBuffers[idx].Create(idx, s_swapChain)) return false;
        if (!Fence::Create(0, D3D12_FENCE_FLAG_NONE, &s_fence[idx])) return false;
        s_constatnBuffers[idx] = std::make_shared<DynamicConstantBuffer>();
        if (!s_constatnBuffers[idx]->Create(CONSTANT_BUFFER_SIZE)) return false;
    }

#if defined(DEBUG) || defined(_DEBUG)
    const auto& windowSize = Window::WindowDebugSize();
#else
    const auto& windowSize = Window::WindowSize();
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

void DirectX12::Update()
{
    // バッファのクリア
    s_prevFrame = s_currentFrame;
    s_currentFrame = s_swapChain->GetCurrentBackBufferIndex();

    s_cmdList[s_currentFrame]->Reset();
    GetConstantBuffer()->ResetBuffer();
}

void DirectX12::BeginDraw()
{
    Barrier(s_backBuffers[s_currentFrame].RenderTargetResource().Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

    const auto& rtvH = s_backBuffers[s_currentFrame].RTVHandle()->CPU();
    CommandList()->OMSetRenderTargets(1, &rtvH, true, nullptr);
    CommandList()->ClearRenderTargetView(rtvH, s_backGroundColor.Raw(), 0, nullptr);

    // ヒープを設定
    SetHeaps();
}

void DirectX12::EndDraw()
{
    Barrier(s_backBuffers[s_currentFrame].RenderTargetResource().Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);

    WaitGPUPrevFrame();
    ExecuteCommandList();
    s_swapChain->Present(0, 0);
}

void DirectX12::Finalize()
{
    WaitGPUThisFrame();
    s_backBuffers.reset();
    s_descriptors[static_cast<int>(PoolType::RES)].reset();
    s_descriptors[static_cast<int>(PoolType::SMP)].reset();
    s_descriptors[static_cast<int>(PoolType::RTV)].reset();
    s_descriptors[static_cast<int>(PoolType::DSV)].reset();
    s_window.Finalize();
}

void DirectX12::ExecuteCommandList()
{
    s_cmdList[s_currentFrame]->CloseList();
    s_cmdList[s_currentFrame]->Execute();
}

void DirectX12::WaitGPU(int frame)
{
    if (0 <= frame || frame > FRAME_COUNT)
    {
        const auto& cmdList = s_cmdList[frame];
        s_fence[frame].Signal(cmdList.get());
        s_fence[frame].WaitGPU();
    }
}

void Glib::Internal::Graphics::DirectX12::WaitGPUThisFrame()
{
    WaitGPU(s_currentFrame);
}

void Glib::Internal::Graphics::DirectX12::WaitGPUPrevFrame()
{
    WaitGPU(s_prevFrame);
}

void DirectX12::SetDefaultRenderTarget() const
{
    const auto& rtvH = s_backBuffers[s_currentFrame].RTVHandle()->CPU();

    CommandList()->OMSetRenderTargets(1, &rtvH, true, nullptr);
    CommandList()->RSSetViewports(1, &s_viewPort);
    CommandList()->RSSetScissorRects(1, &s_scissorRect);
}

void DirectX12::SetHeaps() const
{
    std::array<ID3D12DescriptorHeap* const, 2> heaps{
        s_descriptors[static_cast<UINT>(PoolType::RES)]->GetHeap().Get(),
        s_descriptors[static_cast<UINT>(PoolType::SMP)]->GetHeap().Get()
    };

    CommandList()->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
}

ComPtr<ID3D12Device> DirectX12::Device() const
{
    return s_device;
}

ComPtr<ID3D12GraphicsCommandList> DirectX12::CommandList() const
{
    return s_cmdList[s_currentFrame]->List();
}

ComPtr<ID3D12CommandQueue> DirectX12::CommandQueue() const
{
    return s_cmdQueue->Queue();
}

std::shared_ptr<DescriptorPool> DirectX12::DescriptorPool(PoolType type) const
{
    return s_descriptors[static_cast<int>(type)];
}

std::shared_ptr<DynamicConstantBuffer> DirectX12::GetConstantBuffer()
{
    return s_constatnBuffers[s_currentFrame];
}

D3D12_RESOURCE_DESC DirectX12::BackBufferResourceDesc() const
{
    return s_backBuffers[s_currentFrame].RTVResourceDesc();
}

int DirectX12::BackBufferNum() const
{
    return FRAME_COUNT;
}

void DirectX12::Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const
{
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource, before, after, 0);
    CommandList()->ResourceBarrier(1, &barrier);
}

const Color& DirectX12::BackGroundColor()
{
    return s_backGroundColor;
}

void DirectX12::BackGroundColor(const Color& color)
{
    s_backGroundColor = color;
}

bool DirectX12::InitDevice()
{
    ComPtr<IDXGIAdapter> adapter{ nullptr };
    ComPtr<IDXGIAdapter> nvidiaAdapter{ nullptr };
    ComPtr<IDXGIAdapter> maxVMAdapter{ nullptr };
    size_t videoMemroySize{};
    for (UINT i = 0; s_dxgiFactory->EnumAdapters(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC adptDesc{};
        adapter->GetDesc(&adptDesc);
        std::wstring strDesc{ adptDesc.Description };
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

    constexpr D3D_FEATURE_LEVEL levels[]{
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    for (const auto& level : levels)
    {
        auto result = SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(s_device.ReleaseAndGetAddressOf())));
        if (result) break;
    }
    return s_device != nullptr;
}

bool DirectX12::InitCommand()
{
    // キューの設定
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // キューの作成
    s_cmdQueue = std::make_shared<::CommandQueue>();
    if (!CommandQueue::Create(desc, s_cmdQueue.get())) return false;

    for (size_t i = 0; i < FRAME_COUNT; i++)
    {
        s_cmdAllocator[i] = std::make_shared<::CommandAllocator>();
        s_cmdList[i] = std::make_shared<::CommandList>();

        // アロケーターの作成
        if (!CommandAllocator::Create(desc.Type, s_cmdAllocator[i].get())) return false;

        // リストの作成
        bool result = CommandList::Create(
            desc.Type,
            s_cmdAllocator[i]->Allocator(),
            s_cmdQueue->Queue(),
            s_cmdList[i].get());

        if (!result) return false;
        s_cmdList[i]->CloseList();
    }
    return true;
}

bool DirectX12::CreateSwapChain()
{
#if defined(DEBUG) || defined(_DEBUG)
    const auto& windowSize = Window::WindowDebugSize();
#else
    const auto& windowSize = Window::WindowSize();
#endif

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = static_cast<UINT>(windowSize.x);
    swapChainDesc.Height = static_cast<UINT>(windowSize.y);
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = FRAME_COUNT;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

    // スワップチェインを作成
    ComPtr<IDXGISwapChain1> swapChain{ nullptr };
    auto hr = s_dxgiFactory->CreateSwapChainForHwnd(
        s_cmdQueue->Queue().Get(),
        s_window.WindowHandle(),
        &swapChainDesc,
        nullptr,
        nullptr,
        swapChain.ReleaseAndGetAddressOf()
    );
    if (FAILED(hr)) return false;

    // スワップチェインを変換
    hr = swapChain.As(&s_swapChain);

    return SUCCEEDED(hr);
}

bool DirectX12::CreateDescriptorPool()
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.NodeMask = 1;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.NumDescriptors = RESOURCE_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (!DescriptorPool::Create(&heapDesc, s_descriptors[static_cast<UINT>(PoolType::RES)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    heapDesc.NumDescriptors = SAMPLER_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (!DescriptorPool::Create(&heapDesc, s_descriptors[static_cast<UINT>(PoolType::SMP)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = RENDER_TARGET_VIEW_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (!DescriptorPool::Create(&heapDesc, s_descriptors[static_cast<UINT>(PoolType::RTV)])) return false;

    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    heapDesc.NumDescriptors = DEPTH_STENCIL_VIEW_POOL_SIZE;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (!DescriptorPool::Create(&heapDesc, s_descriptors[static_cast<UINT>(PoolType::DSV)])) return false;

    return true;
}

void DirectX12::EnableDebugLayer()
{
#if defined(DEBUG) || defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController{ nullptr };
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()))))
    {
        debugController->EnableDebugLayer();
    }
#endif
}
