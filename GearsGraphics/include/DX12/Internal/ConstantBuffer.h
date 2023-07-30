#pragma once
#include <memory>
#include <WeakPtr.h>
#include <ComPtr.h>

struct ID3D12Resource;

namespace Glib::Internal::Graphics
{
    class DescriptorHandle;
    class DescriptorPool;
}

namespace Glib::Internal::Graphics
{
    class ConstantBuffer
    {
    public:
        /**
         * @brief �萔�o�b�t�@�[�̍쐬
         * @param bufferSize �o�b�t�@�[�̃T�C�Y
         * @return ���� true
         */
        bool Create(UINT bufferSize);

        /**
         * @brief �萔�o�b�t�@�[�̃o�C���h
         * @param rootParameterIndex
         */
        void BindPipeline(unsigned int rootParameterIndex);

        /**
         * @brief �萔�o�b�t�@�̍X�V
         * @param size �f�[�^�T�C�Y
         * @param data �f�[�^
         */
        void Update(unsigned int size, const void* data);

    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        Glib::WeakPtr<DescriptorPool> pool_{ nullptr };
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
    };
}