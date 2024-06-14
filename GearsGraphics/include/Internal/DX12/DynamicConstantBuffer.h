#pragma once
#include <Internal/DX12/ConstantBufferAddress.h>
#include <ComPtr.h>
#include <memory>
#include <vector>

struct ID3D12Resource;

namespace Glib::Internal::Graphics
{

    /**
     * @brief 動的定数バッファ
     */
    class DynamicConstantBuffer
    {
    public:
        /**
         * @brief バッファの作成をします
         * @brief 注意: データサイズは256バイトでアライメントされます
         * @param bufferSize データサイズ(byte)
         * @return 成功 : true
         * @return 失敗 : false
         */
        bool Create(size_t bufferSize, size_t frameCount);

        /**
         * @brief バッファの値をリセットします
         */
        void ResetBuffer();

        /**
         * @brief バッファにデータを書き込み位置を取得
         * @param data データ
         * @param size サイズ
         * @return 位置
         */
        ConstantBufferAddress Alloc(const void* data, size_t size);

        /**
         * @brief フレームを進める
         */
        void AdvanceFrame();

        /**
         * @brief バッファ全体の開放
         */
        void Release();

    private:
        size_t bufferSize_{ 0 };
        size_t currentBufferIndex_{ 0 };
        size_t frameCount_{ 0 };
        size_t currentFrame_{ 0 };
        std::vector<ComPtr<ID3D12Resource>> buffers_;
    };
}
