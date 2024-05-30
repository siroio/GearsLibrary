#include <Internal/DX12/Fence.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/CommandList.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

Glib::Internal::Graphics::Fence::~Fence()
{
    CloseHandle(fenceEvent_);
}

bool Glib::Internal::Graphics::Fence::Create(const UINT64& initValue, D3D12_FENCE_FLAGS flags, std::shared_ptr<Fence>& fence)
{
    fence = std::make_shared<Fence>();
    if (fence == nullptr) return false;

    // フェンスの作成
    auto hr = s_dx12->Device()->CreateFence(initValue, flags, IID_PPV_ARGS(fence->fence_.ReleaseAndGetAddressOf()));
    fence->fenceValue_ = initValue;
    return SUCCEEDED(hr);
}

void Glib::Internal::Graphics::Fence::Signal(const WeakPtr<CommandList>& cmdList)
{
    cmdList->Queue()->Signal(fence_.Get(), ++fenceValue_);
}

void Glib::Internal::Graphics::Fence::WaitGPU()
{
    if (fence_->GetCompletedValue() < fenceValue_)
    {
        fenceEvent_ = CreateEvent(nullptr, false, false, nullptr);
        fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
        WaitForSingleObject(fenceEvent_, INFINITE);
        CloseHandle(fenceEvent_);
    }
}
