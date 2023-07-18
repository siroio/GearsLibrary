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
         * @brief 定数バッファーの作成
         * @param bufferSize バッファーのサイズ
         * @return 成功 true
         */
        bool Create(UINT bufferSize);


    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        std::shared_ptr<DescriptorPool> pool_{ nullptr };
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
    };
}
