#include <DX12/Internal/IndexBuffer.h>
#include <DX12/Internal/DirectX12.h>
#include <DX12/Internal/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::IndexBuffer::Create(UINT sizePerIndex, UINT indexCount)
{
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(indexCount * static_cast<UINT64>(sizePerIndex));
    auto result = s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(buffer_.ReleaseAndGetAddressOf())
    );

    if (FAILED(result)) return false;

    view_.BufferLocation = buffer_->GetGPUVirtualAddress();
    view_.SizeInBytes = indexCount * sizePerIndex;
    view_.Format = DXGI_FORMAT_R32_UINT;

    return true;
}

void Glib::Internal::Graphics::IndexBuffer::SetBuffer()
{
    s_dx12->CommandList()->IASetIndexBuffer(&view_);
}

void Glib::Internal::Graphics::IndexBuffer::Update(void* indicies)
{
    void* map{ nullptr };
    if (FAILED(buffer_->Map(0, nullptr, &map))) return;
    std::memcpy(map, indicies, view_.SizeInBytes);
    buffer_->Unmap(0, nullptr);
}
