#include <DX12/Internal/VertexBuffer.h>
#include <DX12/Internal/DirectX12.h>
#include <DX12/Internal/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::VertexBuffer::Create(UINT sizePerVetex, UINT vertexCount)
{
    CD3DX12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexCount * static_cast<UINT64>(sizePerVetex));
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
    view_.SizeInBytes = vertexCount * sizePerVetex;
    view_.StrideInBytes = sizePerVetex;

    return true;
}

void Glib::Internal::Graphics::VertexBuffer::SetBuffer()
{

}

void Glib::Internal::Graphics::VertexBuffer::Update(void* vertices)
{
    void* map{ nullptr };
    if (FAILED(buffer_->Map(0, nullptr, &map))) return;
    std::memcpy(map, vertices, view_.SizeInBytes);
    buffer_->Unmap(0, nullptr);
}
