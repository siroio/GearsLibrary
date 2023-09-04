#include <Internal/DX12/DescriptorPool.h>
#include <Internal/DX12/DirectX12.h>
#include <cassert>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

void Glib::Internal::Graphics::DescriptorHandle::CPU(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    cpuHandle_ = handle;
}

void Glib::Internal::Graphics::DescriptorHandle::GPU(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    gpuHandle_ = handle;
}

const D3D12_CPU_DESCRIPTOR_HANDLE& Glib::Internal::Graphics::DescriptorHandle::CPU() const
{
    return cpuHandle_;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& Glib::Internal::Graphics::DescriptorHandle::GPU() const
{
    return gpuHandle_;
}

bool Glib::Internal::Graphics::DescriptorHandle::HasCPU() const
{
    return cpuHandle_.ptr != 0;
}

bool Glib::Internal::Graphics::DescriptorHandle::HasGPU() const
{
    return gpuHandle_.ptr != 0;
}

Glib::Internal::Graphics::DescriptorPool::DescriptorPool() : handles_{}, heap_{}, descriptorSize_{ 0 }
{
    // 初期化用コールバック
    handles_.SetInitializeCallBack([&](size_t index, DescriptorHandle* handle)
    {
        auto cpuHandle = heap_->GetCPUDescriptorHandleForHeapStart();
        cpuHandle.ptr += descriptorSize_ * index;

        auto gpuHandle = heap_->GetGPUDescriptorHandleForHeapStart();
        gpuHandle.ptr += descriptorSize_ * index;

        handle->CPU(cpuHandle);
        handle->GPU(gpuHandle);
    });
}

Glib::Internal::Graphics::DescriptorPool::~DescriptorPool()
{
    handles_.Clear();
    descriptorSize_ = 0;
}

bool Glib::Internal::Graphics::DescriptorPool::Create(const D3D12_DESCRIPTOR_HEAP_DESC* desc, std::shared_ptr<DescriptorPool>& pool)
{
    if (s_dx12->Device() == nullptr || desc == nullptr) return false;
    std::shared_ptr<DescriptorPool> instance{ new DescriptorPool };
    if (instance == nullptr) return false;

    // ディスクリプタヒープ生成
    if (FAILED(s_dx12->Device()->CreateDescriptorHeap(desc, IID_PPV_ARGS(instance->heap_.ReleaseAndGetAddressOf())))) return false;

    // ハンドル加算サイズ取得
    instance->descriptorSize_ = s_dx12->Device()->GetDescriptorHandleIncrementSize(desc->Type);

    // プール初期化
    if (!instance->handles_.Init(desc->NumDescriptors)) return false;

    // プールの設定
    pool = std::move(instance);

    return true;
}

const ComPtr<ID3D12DescriptorHeap> Glib::Internal::Graphics::DescriptorPool::GetHeap() const
{
    return heap_;
}

std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle> Glib::Internal::Graphics::DescriptorPool::GetHandle()
{
    return std::shared_ptr<DescriptorHandle>{ handles_.Get(), [&](DescriptorHandle*& obj)
    {
        Release(obj);
    }};
}

void Glib::Internal::Graphics::DescriptorPool::Release(DescriptorHandle*& handle)
{
    if (handle != nullptr)
    {
        handles_.Release(handle);
        handle = nullptr;
    }
}

size_t Glib::Internal::Graphics::DescriptorPool::UseHeapCount() const
{
    return handles_.UseCount();
}

void Glib::Internal::Graphics::DescriptorPool::Resize(size_t numDescriptors)
{
    ComPtr<ID3D12DescriptorHeap> newHeap;
    D3D12_DESCRIPTOR_HEAP_DESC desc = heap_->GetDesc();
    desc.NumDescriptors = static_cast<UINT>(numDescriptors);
    if (FAILED(s_dx12->Device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(newHeap.ReleaseAndGetAddressOf())))) return;

    // ディスクリプタをコピー
    s_dx12->Device()->CopyDescriptorsSimple(
        heap_->GetDesc().NumDescriptors,
        newHeap->GetCPUDescriptorHandleForHeapStart(),
        heap_->GetCPUDescriptorHandleForHeapStart(),
        desc.Type
    );

    handles_.Expand(static_cast<size_t>(desc.NumDescriptors - heap_->GetDesc().NumDescriptors));

    // 新しいヒープに既存のハンドルをコピー
    auto result = handles_.Init([&](size_t index, DescriptorHandle* handle)
    {
        auto cpuHandle = newHeap->GetCPUDescriptorHandleForHeapStart();
        cpuHandle.ptr += descriptorSize_ * index;

        auto gpuHandle = newHeap->GetGPUDescriptorHandleForHeapStart();
        gpuHandle.ptr += descriptorSize_ * index;

        handle->CPU(cpuHandle);
        handle->GPU(gpuHandle);
    });

    if (!result) throw std::bad_alloc{};
    heap_.Swap(newHeap);
}
