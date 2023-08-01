#pragma once
#include <d3d12.h>
#include <ComPtr.h>
#include <ObjectPool.h>
#include <atomic>
#include <memory>

namespace Glib::Internal::Graphics
{
    /**
     * @brief ディスクリプタハンドル
     */
    class DescriptorHandle
    {
    public:
        void CPU(D3D12_CPU_DESCRIPTOR_HANDLE handle);
        void GPU(D3D12_GPU_DESCRIPTOR_HANDLE handle);
        const D3D12_CPU_DESCRIPTOR_HANDLE& CPU() const;
        const D3D12_GPU_DESCRIPTOR_HANDLE& GPU() const;
        bool HasCPU() const;
        bool HasGPU() const;

    private:
        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_;
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_;
    };


    class DescriptorPool
    {
    public:
        /**
         * @brief プールを作成
         * @param desc ヒープの設定
         * @param pool プール変数への参照
         * @return true 成功
         * @return false 失敗
        */
        static bool Create(const D3D12_DESCRIPTOR_HEAP_DESC* desc, std::shared_ptr<DescriptorPool>& pool);

    public:
        ~DescriptorPool();

        /**
         * @brief ヒープの取得
         */
        const ComPtr<ID3D12DescriptorHeap> GetHeap() const;

        /**
         * @brief ハンドルをプールから取得
         * @brief 参照がない場合スコープを抜けると解放
         */
        std::shared_ptr<DescriptorHandle> GetHandle();

        /**
         * @brief ハンドルを解放
         * @param handle
         */
        void Release(DescriptorHandle*& handle);

        /**
         * @brief 利用中のヒープの数
         */
        size_t UseHeapCount() const;

    private:
        void Resize(size_t numDescriptors);

    private:
        DescriptorPool();
        DescriptorPool(const DescriptorPool&) = delete;
        void operator = (const DescriptorPool&) = delete;

    private:
        ObjectPool<DescriptorHandle> handles_;
        ComPtr<ID3D12DescriptorHeap> heap_;
        uint32_t descriptorSize_;
    };
}
