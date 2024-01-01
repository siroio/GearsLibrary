#include <Internal/DX12/DescriptorHandle.h>

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

