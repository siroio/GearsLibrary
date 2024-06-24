#pragma once
#include <memory>
#include <WeakPtr.h>
#include <ComPtr.h>
#include <initializer_list>

struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct D3D12_COMMAND_QUEUE_DESC;
enum D3D12_COMMAND_LIST_TYPE;

namespace Glib::Internal::Graphics
{
    /**
     * @brief コマンドリスト管理クラス
     */
    class CommandList
    {
    public:
        CommandList() = default;

        /**
         * @brief コマンドリストの作成
         * @return 成功 true
         * @return 失敗 false
         */
        static bool Create(D3D12_COMMAND_LIST_TYPE type, ComPtr<ID3D12CommandAllocator> allocator, ComPtr<ID3D12CommandQueue> queue, CommandList* cmdList);

        /**
         * @brief バンドル用のコマンドリストを作成
         * @return 成功 true
         * @return 失敗 false
         */
        static bool CreateBundle(CommandList* cmdList);

    public:
        /**
         * @brief コマンドの実行
         */
        void Execute() const;

        /**
         * @brief バンドルしたコマンドをリストに追加
         */
        bool CloseAndBundleToList(const CommandList* lists) const;

        /**
         * @brief リストのクローズ
         */
        void CloseList();

        /**
         * @brief リストのリセット
         */
        void Reset();

        /**
         * @brief アロケーターの取得
         */
        ComPtr<ID3D12CommandAllocator> Allocator() const;

        /**
         * @brief リストの取得
         */
        ComPtr<ID3D12GraphicsCommandList> List() const;

        /**
         * @brief キューの取得
         */
        ComPtr<ID3D12CommandQueue> Queue() const;

        /**
         * @brief コマンドリストのタイプを取得
         * @return タイプ
         */
        D3D12_COMMAND_LIST_TYPE Type() const;

    private:
        CommandList(const CommandList&) = delete;
        CommandList& operator = (const CommandList&) = delete;

    private:
        ComPtr<ID3D12CommandAllocator> cmdAllocator_{ nullptr };
        ComPtr<ID3D12GraphicsCommandList> cmdList_{ nullptr };
        ComPtr<ID3D12CommandQueue> cmdQueue_{ nullptr };
    };
}
