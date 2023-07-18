#pragma once
#include <memory>
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


    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        std::shared_ptr<DescriptorPool> pool_{ nullptr };
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
    };
}
