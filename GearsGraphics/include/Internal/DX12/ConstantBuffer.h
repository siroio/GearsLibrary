#pragma once
#include <ComPtr.h>
#include <memory>
#include <d3d12.h>

namespace Glib::Internal::Graphics
{
    class DescriptorHandle;
}

namespace Glib::Internal::Graphics
{
    /**
     * @brief �萔�o�b�t�@�[
     */
    class ConstantBuffer
    {
    public:
        /**
         * @brief �萔�o�b�t�@�[�̍쐬
         * @param bufferSize �o�b�t�@�[�̃T�C�Y
         * @return ���� true
         */
        bool Create(UINT64 bufferSize);

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
        ComPtr<ID3D12Resource> buffer_;
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
    };
}
