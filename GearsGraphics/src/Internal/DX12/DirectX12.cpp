#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/CommandList.h>
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

    /* コマンドリスト */
    std::array<std::shared_ptr<CommandList>, FRAME_COUNT> s_cmdList{};

    /* フェンス */
    std::array<Fence, FRAME_COUNT> s_fence{};

    constexpr unsigned int CONSTANT_BUFFER_SIZE{ 256 * 2000 };

    /* ディスクリプタプール */
    std::array<std::shared_ptr<DescriptorPool>,
        static_cast<int>(DirectX12::PoolType::COUNT)> s_descriptors;

    /* 動的定数バッファ */
    std::shared_ptr<DynamicConstantBuffer> s_constatnBuffer;

    /* 描画フレーム */
    unsigned int s_prevFrame{ 0 };
    unsigned int s_currentFrame{ 0 };

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
    }

    s_constatnBuffer = std::make_shared<DynamicConstantBuffer>();
    if (!s_constatnBuffer->Create(CONSTANT_BUFFER_SIZE, FRAME_COUNT)) return false;

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

void DirectX12::BeginDraw()
{
    s_prevFrame = s_currentFrame;
    s_currentFrame = s_swapChain->GetCurrentBackBufferIndex();

    s_constatnBuffer->ResetBuffer();
    s_cmdList[s_currentFrame]->Reset();

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

    ExecuteCommandList();
    s_swapChain->Present(0, 0);
    s_constatnBuffer->AdvanceFrame();
}

void DirectX12::Finalize()
{
    WaitGPU(s_currentFrame);
    s_backBuffers.reset();
    s_descriptors[static_cast<int>(PoolType::RES)].reset();
    s_descriptors[static_cast<int>(PoolType::SMP)].reset();
    s_descriptors[static_cast<int>(PoolType::RTV)].reset();
    s_descriptors[static_cast<int>(PoolType::DSV)].reset();
    s_window.Finalize();
}

void DirectX12::ExecuteCommandList()
{
    WaitGPU(s_prevFrame);
    s_cmdList[s_currentFrame]->CloseList();
    s_cmdList[s_currentFrame]->Execute();
}

void DirectX12::WaitGPU(int frame)
{
    if (0 <= frame || frame > FRAME_COUNT)
    {
        s_fence[frame].Signal(s_cmdList[frame].get());
        s_fence[frame].WaitGPU();
    }
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
    return s_cmdList[s_currentFrame]->Queue();
}

std::shared_ptr<DescriptorPool> DirectX12::DescriptorPool(PoolType type) const
{
    return s_descriptors[static_cast<int>(type)];
}

std::shared_ptr<DynamicConstantBuffer> DirectX12::GetConstantBuffer()
{
    return s_constatnBuffer;
}

D3D12_RESOURCE_DESC DirectX12::BackBufferResourceDesc() const
{
    return s_backBuffers[0].RTVResourceDesc();
}

int DirectX12::BackBufferNum() const
{
    return FRAME_COUNT;
}

void DirectX12::Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
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

    for (const auto& level : levels)
    {
        auto result = SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(s_device.ReleaseAndGetAddressOf())));
        if (result) break;
    }
    return s_device != nullptr;
}

bool DirectX12::InitCommand()
{
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmdQueueDesc.NodeMask = 0;
    cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    for (size_t i = 0; i < FRAME_COUNT; i++)
    {
        s_cmdList[i] = std::make_shared<::CommandList>();
        if (!CommandList::Create(D3D12_COMMAND_LIST_TYPE_DIRECT, cmdQueueDesc, s_cmdList[i].get()))
        {
            return false;
        }
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

    return SUCCEEDED(s_dxgiFactory->CreateSwapChainForHwnd(
        CommandQueue().Get(),
        s_window.WindowHandle(),
        &swapChainDesc,
        nullptr,
        nullptr,
        reinterpret_cast<IDXGISwapChain1**>(s_swapChain.ReleaseAndGetAddressOf())
    ));
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
