#include <Internal/DX12/CommandQueue.h>
#include <Internal/DX12/DirectX12.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::CommandQueue::Create(const D3D12_COMMAND_QUEUE_DESC& desc, CommandQueue* cmdQueue)
{
    if (cmdQueue == nullptr) return false;

    // コマンドキューの作成
    auto hr = s_dx12->Device()->CreateCommandQueue(
        &desc, IID_PPV_ARGS(cmdQueue->cmdQueue_.ReleaseAndGetAddressOf())
    );

    return SUCCEEDED(hr);
}

ComPtr<ID3D12CommandQueue> Glib::Internal::Graphics::CommandQueue::Queue() const
{
    return cmdQueue_;
}
