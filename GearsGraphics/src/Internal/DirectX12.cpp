#include <Internal/DirectX12.h>
#include <Window.h>
#include <Color.h>
#include <Vector2.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <string>
#include <array>

//TODO: imple DirectX12 class
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

    /* 背景色 */
    Color backGroundColor_ = Color::Cyan();

    /* ウィンドウ */
    Glib::Window& window_ = Glib::Window::Instance();
}

bool Glib::Internal::Graphics::DirectX12::Initialize()
{
    if (!window_.Initialize()) return false;
    EnableDebugLayer();
    // ファクトリーの作成
#ifdef _DEBUG
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(dxgiFactory_.ReleaseAndGetAddressOf())))) return false;
#else
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory_.ReleaseAndGetAddressOf())))) return false;
#endif
    if (!InitDevice()) return false;
    if (!InitCommand()) return false;
    if (!CreateSwapChain()) return false;
    return true;
}

void Glib::Internal::Graphics::DirectX12::BeginDraw()
{}

void Glib::Internal::Graphics::DirectX12::EndDraw()
{}

void Glib::Internal::Graphics::DirectX12::Finalize()
{}

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
    auto result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator_.ReleaseAndGetAddressOf()));
    if (FAILED(result)) return false;
    result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(cmdList_.ReleaseAndGetAddressOf()));
    if (FAILED(result)) return false;

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
#ifdef _DEBUG
    const auto& windowSize = Window::WindowSize();
#else
    const auto& windowSize = Window::WindowSize();
#endif

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = static_cast<unsigned int>(windowSize.x);
    swapChainDesc.Height = static_cast<unsigned int>(windowSize.y);
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

void Glib::Internal::Graphics::DirectX12::EnableDebugLayer()
{
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController{ nullptr };
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()))))
    {
        debugController->EnableDebugLayer();
    }
#endif
}
