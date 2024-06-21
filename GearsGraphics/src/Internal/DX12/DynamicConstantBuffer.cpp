#include <Internal/DX12/DynamicConstantBuffer.h>
#include <Internal/DX12/DescriptorHandle.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::DynamicConstantBuffer::Create(size_t bufferSize, size_t frameCount)
{
    bufferSize_ = (bufferSize + 0xff) & ~0xff;
    frameCount_ = frameCount;
    currentFrame_ = 0;
    buffers_.resize(frameCount_);

    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize_); // 256バイトでアライメント

    for (size_t i = 0; i < frameCount_; ++i)
    {
        auto result = s_dx12->Device()->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(buffers_[i].ReleaseAndGetAddressOf())
        );

        if (FAILED(result)) return false;
    }
    return true;
}

void Glib::Internal::Graphics::DynamicConstantBuffer::ResetBuffer()
{
    if (buffers_.empty()) return;
    void* data = nullptr;
    buffers_[currentFrame_]->Map(0, nullptr, &data);
    std::memset(data, 0, bufferSize_);
    currentBufferIndex_ = 0;
    buffers_[currentFrame_]->Unmap(0, nullptr);
}

Glib::Internal::Graphics::ConstantBufferAddress Glib::Internal::Graphics::DynamicConstantBuffer::Alloc(const void* data, size_t size)
{
    size_t alignedSize = (size + 0xff) & ~0xff; // サイズをアライメント
    size_t startOffset = currentBufferIndex_;

    // バッファがない場合や書き込み先がオーバーフローする場合は無効なアドレスを返す
    if (buffers_.empty() || (currentBufferIndex_ + alignedSize) > bufferSize_) return ConstantBufferAddress{};

    // 書き込み開始
    void* map{ nullptr };
    if (FAILED(buffers_[currentFrame_]->Map(0, nullptr, &map))) return ConstantBufferAddress{};
    std::memcpy(static_cast<std::byte*>(map) + startOffset, data, size);
    currentBufferIndex_ += alignedSize;
    buffers_[currentFrame_]->Unmap(0, nullptr);

    // アドレスの開始位置を計算して返す
    ConstantBufferAddress address{};
    address.address_ = buffers_[currentFrame_]->GetGPUVirtualAddress() + startOffset;
    return address;
}

void Glib::Internal::Graphics::DynamicConstantBuffer::AdvanceFrame()
{
    currentFrame_ = ++currentFrame_ % frameCount_;
}

void Glib::Internal::Graphics::DynamicConstantBuffer::Release()
{
    buffers_.clear();
}
