#pragma once
#include <d3d12.h>
#include <atomic>
#include <ComPtr.h>
#include <ObjectPool.h>

namespace Glib::Graphics::Internal
{
    /**
     * @brief �f�B�X�N���v�^�n���h��
     */
    class DescriptorHandle
    {
    public:
        void CPU(D3D12_CPU_DESCRIPTOR_HANDLE handle);
        void GPU(D3D12_GPU_DESCRIPTOR_HANDLE handle);
        const D3D12_CPU_DESCRIPTOR_HANDLE& CPU() const;
        const D3D12_GPU_DESCRIPTOR_HANDLE& GPU() const;
        bool HasCPU() const;
        bool HasGPU() const;

    private:
        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_;
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_;
    };


    class DescriptorPool
    {

    };
}
