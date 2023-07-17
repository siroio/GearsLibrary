#pragma once
#include <d3d12.h>
#include <atomic>
#include <ComPtr.h>
#include <ObjectPool.h>

namespace Glib::Internal::Graphics
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
    private:
        DescriptorPool();
        ~DescriptorPool();
        DescriptorPool(const DescriptorPool&) = delete;
        void operator = (const DescriptorPool&) = delete;

    public:
        /**
         * @brief �v�[�����쐬
         * @param device �f�o�C�X
         * @param desc �q�[�v�̐ݒ�
         * @param pool �v�[���ϐ��ւ̎Q��
         * @return true ����
         * @return false ���s
        */
        static bool Create(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC* desc, std::shared_ptr<DescriptorPool>& pool);

    public:
        const ComPtr<ID3D12DescriptorHeap> GetHeap() const;
        DescriptorHandle* GetHandle();
        void Release();
        void Release(DescriptorHandle*& handle);
        void IncrementRef();
        uint32_t Count() const;

    private:
        ObjectPool<DescriptorHandle> handles_;
        std::atomic<uint32_t> refCount_;
        ComPtr<ID3D12DescriptorHeap> heap_;
        uint32_t descriptorSize_;
    };
}
