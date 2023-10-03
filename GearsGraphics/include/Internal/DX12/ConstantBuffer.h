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
     * @brief 定数バッファー
     */
    class ConstantBuffer
    {
    public:
        /**
         * @brief 定数バッファーの作成
         * @param bufferSize バッファーのサイズ
         * @return 成功 true
         */
        bool Create(UINT64 bufferSize);

        /**
         * @brief 定数バッファーのバインド
         * @param rootParameterIndex
         */
        void BindPipeline(unsigned int rootParameterIndex);

        /**
         * @brief 定数バッファの更新
         * @param size データサイズ
         * @param data データ
         */
        void Update(unsigned int size, const void* data);

    private:
        ComPtr<ID3D12Resource> buffer_;
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
    };
}
