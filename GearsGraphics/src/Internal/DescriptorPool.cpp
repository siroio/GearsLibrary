#include <Internal/DescriptorPool.h>

void Glib::Graphics::Internal::DescriptorHandle::CPU(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    cpuHandle_ = handle;
}

void Glib::Graphics::Internal::DescriptorHandle::GPU(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    gpuHandle_ = handle;
}

const D3D12_CPU_DESCRIPTOR_HANDLE& Glib::Graphics::Internal::DescriptorHandle::CPU() const
{
    return cpuHandle_;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& Glib::Graphics::Internal::DescriptorHandle::GPU() const
{
    return gpuHandle_;
}

bool Glib::Graphics::Internal::DescriptorHandle::HasCPU() const
{
    return cpuHandle_.ptr != 0;
}

bool Glib::Graphics::Internal::DescriptorHandle::HasGPU() const
{
    return gpuHandle_.ptr != 0;
}
