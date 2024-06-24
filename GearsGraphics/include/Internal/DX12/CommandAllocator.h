#pragma once
#include <ComPtr.h>

struct ID3D12CommandAllocator;
enum D3D12_COMMAND_LIST_TYPE;

namespace Glib::Internal::Graphics
{
    class CommandAllocator
    {
    public:
        CommandAllocator() = default;

        /**
         * @brief コマンドアロケーターの作成
         * @param type 種類
         */
        static bool Create(D3D12_COMMAND_LIST_TYPE type, CommandAllocator* allocator);

        /**
         * @brief アロケーターの取得
         */
        ComPtr<ID3D12CommandAllocator> Allocator() const;

    private:
        CommandAllocator(const CommandAllocator&) = delete;
        CommandAllocator& operator = (const CommandAllocator&) = delete;

    private:
        ComPtr<ID3D12CommandAllocator> cmdAllocator_{ nullptr };
    };
}
