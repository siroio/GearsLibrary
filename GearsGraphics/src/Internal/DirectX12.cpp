#include <Internal/DirectX12.h>
#include <Window.h>
#include <ComPtr.h>
#include <Color.h>
#include <Vector2.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

//TODO: imple DirectX12 class
//TODO: pragmaでライブラリをリンクするヘッダ作成
namespace
{
    ComPtr<ID3D12Device> device_;

    /* スワップチェーン */
    ComPtr<IDXGISwapChain4> swapChain_;

    /* コマンドキュー */
    ComPtr<ID3D12CommandQueue> commandQueue_;

    /* ファクトリー */
    ComPtr<IDXGIFactory6> factory_;

    /* ウィンドウ */
    Glib::Window& window_ = Glib::Window::Instance();

    /* 背景色 */
    Color backGroundColor_;
}

bool Glib::Internal::Graphics::DirectX12::Initialize()
{
    if (!window_.Initialize()) return false;
    EnableDebugLayer();
    // ファクトリーの作成
#ifdef _DEBUG
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(factory_.ReleaseAndGetAddressOf())))) return false;
#else
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(factory_.ReleaseAndGetAddressOf())))) return false;
#endif

    if (!InitDevice()) return false;
    if (!CreateSwapChain()) return false;
    return true;
}

void Glib::Internal::Graphics::DirectX12::BeginDraw()
{}

void Glib::Internal::Graphics::DirectX12::EndDraw()
{}

void Glib::Internal::Graphics::DirectX12::Finalize()
{}

const Color& Glib::Internal::Graphics::DirectX12::BackGroundColor()
{
    return backGroundColor_;
}

void Glib::Internal::Graphics::DirectX12::BackGroundColor(const Color& color)
{}

bool Glib::Internal::Graphics::DirectX12::InitDevice()
{
    return false;
}

bool Glib::Internal::Graphics::DirectX12::CreateSwapChain()
{
#ifdef _DEBUG
    const auto& windowSize = Window::WindowSize();
#else
    const auto& windowSize = Window::WindowSize();
#endif

    // スワップチェーン作成
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

    return SUCCEEDED(factory_->CreateSwapChainForHwnd(
        commandQueue_.Get(),
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
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
#endif
}
