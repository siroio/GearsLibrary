#include <Internal/ConstantBuffer.h>
#include <Internal/d3dx12Inc.h>
#include <Internal/DirectX12.h>
#include <Internal/DescriptorPool.h>

namespace Graphics = Glib::Internal::Graphics;

namespace
{
    auto dx12 = Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::ConstantBuffer::Create(UINT bufferSize)
{
    // バッファー作成
    CD3DX12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((static_cast<UINT64>(bufferSize) + 0xff) & ~0xff);

    auto result = dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(buffer_.ReleaseAndGetAddressOf())
    );

    if (FAILED(result)) return false;

    // ハンドル取得
    pool_ = dx12->DescriptorPool(DirectX12::POOLTYPE::RES);
    handle_ = pool_->GetHandle();

    // viewの作成
    D3D12_CONSTANT_BUFFER_VIEW_DESC viewDesc{};
    viewDesc.BufferLocation = buffer_->GetGPUVirtualAddress();
    viewDesc.SizeInBytes = (bufferSize + 0xff) & ~0xff;
    return true;
}
