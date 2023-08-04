#pragma once
#include <d3d12.h>
#include <ComPtr.h>

namespace Glib::Internal::Graphics
{
    /**
     * @brief 頂点バッファー
     */
    class VertexBuffer
    {
    public:
        /**
         * @brief 頂点バッファーの作成
         * @param sizePerIndex 頂点ごとのサイズ
         * @param indexCount 頂点のサイズ
         * @return 成功 true
         */
        bool Create(unsigned int sizePerVetex, unsigned int vertexCount);

        /**
         * @brief 頂点バッファーをセット
         * @param rootParameterIndex
         */
        void SetBuffer();

        /**
         * @brief 頂点バッファの更新
         * @param size データサイズ
         * @param data データ
         */
        void Update(void* vertices);

    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        D3D12_VERTEX_BUFFER_VIEW view_{};
    };
}
