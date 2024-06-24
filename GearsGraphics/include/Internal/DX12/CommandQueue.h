#pragma once
#include <ComPtr.h>

struct ID3D12CommandQueue;
struct D3D12_COMMAND_QUEUE_DESC;

namespace Glib::Internal::Graphics
{
    class CommandQueue
    {
    public:
        CommandQueue() = default;

        /**
         * @brief キューの作成
         * @param desc 設定
         */
        static bool Create(const D3D12_COMMAND_QUEUE_DESC& desc, CommandQueue* cmdQueue);

        /**
         * @brief キューの取得
         */
        ComPtr<ID3D12CommandQueue> Queue() const;

    private:
        ComPtr<ID3D12CommandQueue> cmdQueue_{ nullptr };
    };
}
