#pragma once
#include <d3d12.h>
#include <Internal/DX12/DescriptorHandle.h>
#include <ComPtr.h>
#include <ObjectPool.h>
#include <atomic>
#include <memory>

namespace Glib::Internal::Graphics
{
    /**
     * @brief �f�B�X�N���v�^�v�[��
     */
    class DescriptorPool
    {
    public:
        /**
         * @brief �v�[�����쐬
         * @param desc �q�[�v�̐ݒ�
         * @param pool �v�[���ϐ��ւ̎Q��
         * @return true ����
         * @return false ���s
        */
        static bool Create(const D3D12_DESCRIPTOR_HEAP_DESC* desc, std::shared_ptr<DescriptorPool>& pool);

    public:
        ~DescriptorPool();

        /**
         * @brief �q�[�v�̎擾
         */
        const ComPtr<ID3D12DescriptorHeap> GetHeap() const;

        /**
         * @brief �n���h�����v�[������擾
         * @brief �Q�Ƃ��Ȃ��ꍇ�X�R�[�v�𔲂���Ɖ��
         */
        std::shared_ptr<DescriptorHandle> GetHandle();

        /**
         * @brief �n���h�������
         * @param handle
         */
        void Release(DescriptorHandle*& handle);

        /**
         * @brief ���p���̃q�[�v�̐�
         */
        size_t UseHeapCount() const;

    private:
        void Resize(size_t numDescriptors);

    private:
        DescriptorPool();
        DescriptorPool(const DescriptorPool&) = delete;
        void operator = (const DescriptorPool&) = delete;

    private:
        ObjectPool<DescriptorHandle> handles_;
        ComPtr<ID3D12DescriptorHeap> heap_;
        uint32_t descriptorSize_;
    };
}
