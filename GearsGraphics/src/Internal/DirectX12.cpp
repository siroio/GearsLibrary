#include <Internal/DirectX12.h>
#include <Window.h>
#include <ComPtr.h>
#include <Color.h>
#include <Vector2.h>
#include <dxgi1_6.h>
//TODO: imple DirectX12 class
namespace
{
    Color bg_color{ 0.0f };
    ComPtr<IDXGISwapChain4> swapChain_;
    ComPtr<ID3D12CommandQueue> commandQueue_;
    ComPtr<IDXGIFactory6> factory_;
    Glib::Window& window_ = Glib::Window::Instance();
}

bool Glib::Internal::Graphics::DirectX12::Initialize()
{
    if (!window_.Initialize()) return false;
    EnableDebugLayer();


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
    return bg_color;
}

void Glib::Internal::Graphics::DirectX12::BackGroundColor(const Color& color)
{}

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
    factory_->CreateSwapChainForHwnd(
        commandQueue_.Get(),
        window_.WindowHandle(),
        &swapChainDesc,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)swapChain_.ReleaseAndGetAddressOf()
    );
    return true;
}

void Glib::Internal::Graphics::DirectX12::EnableDebugLayer()
{
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debuglayer{ nullptr };
    if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(debuglayer.GetAddressOf())))) return;
    debuglayer->EnableDebugLayer();
#endif
}
