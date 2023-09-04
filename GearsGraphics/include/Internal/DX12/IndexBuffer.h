#pragma once
#include <d3d12.h>
#include <ComPtr.h>

namespace Glib::Internal::Graphics
{
    /**
     * @brief インデックスバッファー
     */
    class IndexBuffer
    {
    public:
        /**
         * @brief インデックスバッファーの作成
         * @param sizePerIndex インデックスごとのサイズ
         * @param indexCount インデックスのサイズ
         * @return 成功 true
         */
        bool Create(unsigned int sizePerIndex, unsigned int indexCount);

        /**
         * @brief インデックスバッファーをセット
         * @param rootParameterIndex
         */
        void SetBuffer();

        /**
         * @brief インデックスバッファの更新
         * @param size データサイズ
         * @param data データ
         */
        void Update(void* indicies);

    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        D3D12_INDEX_BUFFER_VIEW view_{};
    };
}
