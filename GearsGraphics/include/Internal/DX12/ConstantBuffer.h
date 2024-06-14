#pragma once
#include <ComPtr.h>
#include <memory>

struct ID3D12Resource;

namespace Glib::Internal::Graphics
{
    class DescriptorHandle;

    /**
     * @brief 定数バッファー
     */
    class ConstantBuffer
    {
    public:
        ~ConstantBuffer();

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
        void SetBuffer(unsigned int rootParameterIndex) const;

        /**
         * @brief 定数バッファの更新
         * @param size データサイズ
         * @param data データ
         */
        void Update(const void* data, unsigned int size);

        /**
         * @brief バッファを解放
         */
        void Release();

    private:
        ComPtr<ID3D12Resource> buffer_;
        std::shared_ptr<DescriptorHandle> handle_{ nullptr };
    };
}
