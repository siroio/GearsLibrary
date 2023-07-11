#include <Internal/DescriptorPool.h>

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

Glib::Internal::Graphics::DescriptorPool::DescriptorPool() : refCount_{ 1 }, handles_{}, heap_{}, descriptorSize_{ 0 }
{
    auto init = [&](size_t index, DescriptorHandle* handle)
    {
        auto cpuHandle = heap_->GetCPUDescriptorHandleForHeapStart();
        cpuHandle.ptr += descriptorSize_ * index;

        auto gpuHandle = heap_->GetGPUDescriptorHandleForHeapStart();
        gpuHandle.ptr += descriptorSize_ * index;

        handle->CPU(cpuHandle);
        handle->GPU(gpuHandle);
    };

    handles_.SetInitializeCallBack(init);
}

Glib::Internal::Graphics::DescriptorPool::~DescriptorPool()
{
    handles_.Clear();
    heap_.Reset();
    descriptorSize_ = 0;
}

bool Glib::Internal::Graphics::DescriptorPool::Create(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC* desc, DescriptorPool** pool)
{
    if (device == nullptr || desc == nullptr || pool == nullptr) return false;

    auto instance = new DescriptorPool{};
    if (instance == nullptr) return false;

    // ディスクリプタヒープ生成
    if (FAILED(device->CreateDescriptorHeap(desc, IID_PPV_ARGS(instance->heap_.GetAddressOf()))))
    {
        instance->Release();
        return false;
    }

    // 加算サイズ取得
    instance->descriptorSize_ = device->GetDescriptorHandleIncrementSize(desc->Type);

    // プール初期化
    if (!instance->handles_.Init(desc->NumDescriptors))
    {
        instance->Release();
        return false;
    }

    // プールの設定
    *pool = instance;

    return true;
}

const ComPtr<ID3D12DescriptorHeap> Glib::Internal::Graphics::DescriptorPool::GetHeap() const
{
    return heap_;
}

Glib::Internal::Graphics::DescriptorHandle* Glib::Internal::Graphics::DescriptorPool::GetHandle()
{
    return handles_.Get();
}

void Glib::Internal::Graphics::DescriptorPool::Release()
{
    refCount_--;
    if (refCount_ == 0) delete this;
}

void Glib::Internal::Graphics::DescriptorPool::Release(DescriptorHandle*& handle)
{
    if (handle != nullptr)
    {
        handles_.Release(handle);
        handle = nullptr;
    }
}

void Glib::Internal::Graphics::DescriptorPool::IncrementRef()
{
    refCount_++;
}

uint32_t Glib::Internal::Graphics::DescriptorPool::Count() const
{
    return refCount_;
}
