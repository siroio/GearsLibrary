#pragma once
#include <d3d12.h>
#include <ComPtr.h>

namespace Glib::Internal::Graphics
{
    class IndexBuffer
    {
    public:
        /**
         * @brief インデックスバッファーの作成
         * @param sizePerIndex インデックスごとのサイズ
         * @param indexCount インデックスのサイズ
         * @return 成功 true
         */
        bool Create(UINT sizePerIndex, UINT indexCount);

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
