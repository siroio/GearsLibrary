#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/DescriptorPool.h>

namespace Graphics = Glib::Internal::Graphics;

namespace
{
    auto s_dx12 = Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::ConstantBuffer::Create(unsigned int bufferSize)
{
    // �o�b�t�@�[�쐬
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((static_cast<UINT64>(bufferSize) + 0xff) & ~0xff);

    auto result = s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(buffer_.ReleaseAndGetAddressOf())
    );

    if (FAILED(result)) return false;

    // �n���h���擾
    pool_ = s_dx12->DescriptorPool(DirectX12::PoolType::RES);
    handle_ = pool_->GetHandle();

    // view�̍쐬
    D3D12_CONSTANT_BUFFER_VIEW_DESC viewDesc{};
    viewDesc.BufferLocation = buffer_->GetGPUVirtualAddress();
    viewDesc.SizeInBytes = (bufferSize + 0xff) & ~0xff;
    s_dx12->Device()->CreateConstantBufferView(&viewDesc, handle_->CPU());
    return true;
}

void Glib::Internal::Graphics::ConstantBuffer::BindPipeline(unsigned int rootParameterIndex)
{
    s_dx12->CommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, handle_->GPU());
}

void Glib::Internal::Graphics::ConstantBuffer::Update(unsigned int size, const void* data)
{
    void* map{ nullptr };
    if (FAILED(buffer_->Map(0, nullptr, &map))) return;
    std::memcpy(map, data, size);
    buffer_->Unmap(0, nullptr);
}