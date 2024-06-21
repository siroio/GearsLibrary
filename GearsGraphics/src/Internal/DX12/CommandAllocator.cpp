#include <Internal/DX12/CommandAllocator.h>
#include <Internal/DX12/DirectX12.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::CommandAllocator::Create(D3D12_COMMAND_LIST_TYPE type, CommandAllocator* allocator)
{
    if (allocator == nullptr) return false;

    // アロケーター作成
    auto hr = s_dx12->Device()->CreateCommandAllocator(
        type, IID_PPV_ARGS(allocator->cmdAllocator_.ReleaseAndGetAddressOf())
    );

    return SUCCEEDED(hr);
}

ComPtr<ID3D12CommandAllocator> Glib::Internal::Graphics::CommandAllocator::Allocator() const
{
    return cmdAllocator_;
}
