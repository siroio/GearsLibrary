#pragma once
#include <ComPtr.h>
#include <vector>
#include <memory>
#include <WeakPtr.h>

struct ID3D12Device;
struct ID3D12Fence;
enum D3D12_FENCE_FLAGS;

namespace Glib::Internal::Graphics
{
    class CommandList;

    class Fence
    {
    public:
        Fence() = default;
        ~Fence();

        /**
         * @brief フェンスの作成
         * @param initValue 初期値
         * @param flag フラグ
         * @return
         */
        static bool Create(const UINT64& initValue, D3D12_FENCE_FLAGS flags, std::shared_ptr<Fence>& fence);

        void Signal(const WeakPtr<CommandList>& cmdList);

        void WaitGPU();

    private:
        ComPtr<ID3D12Fence> fence_{ nullptr };
        UINT64 fenceValue_{ 0 };
        HANDLE fenceEvent_{ nullptr };
    };
}
