#pragma once
#include<ComPtr.h>

struct ID3D12Resource;

namespace Glib::Internal::Graphics
{
    /**
     * @brief 定数バッファー
    */
    class ConstantBuffer
    {
    public:
        bool Create(UINT64 bufferSize);
    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
    };
}