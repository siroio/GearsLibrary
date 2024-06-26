﻿#pragma once

typedef unsigned long long UINT64;
typedef UINT64 D3D12_GPU_VIRTUAL_ADDRESS;

namespace Glib::Internal::Graphics
{
    /**
     * @brief 定数バッファのアドレスを指す
     */
    struct ConstantBufferAddress
    {
        friend class DynamicConstantBuffer;

        ConstantBufferAddress() = default;

        /**
         * @brief アドレスを取得
         */
        D3D12_GPU_VIRTUAL_ADDRESS Address() const;

    private:
        D3D12_GPU_VIRTUAL_ADDRESS address_;
    };
}
