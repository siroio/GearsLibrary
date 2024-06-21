#pragma once
#include <Singleton.h>
#include <Internal/ISystem.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
#include <Internal/DX12/DescriptorPool.h>
#include <ComPtr.h>

struct Color;
struct ID3D12Device;
struct IDXGIFactory6;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct D3D12_RESOURCE_DESC;

namespace Glib::Internal::Graphics
{
    class DynamicConstantBuffer;

    class DirectX12 :
        public SingletonPtr<DirectX12>,
        public Interface::ISystem,
        public Function::InitializeOrderSet<Order::Initialzie::DX12>,
        public Function::BeginDrawOrderSet<Order::BeginDraw::DX12>
    {
        DirectX12() = default;
        friend WeakPtr<DirectX12> SingletonPtr<DirectX12>::Instance();

    public:
        /**
         * @brief DescriptorPoolの種類
         */
        enum class PoolType : unsigned int
        {
            RES = 0,
            SMP = 1,
            RTV = 2,
            DSV = 3,
            COUNT = 4,
        };

    public:

        /**
         * @brief 初期化
         */
        bool Initialize();

        /**
         * @brief 描画開始
         */
        void BeginDraw();

        /**
         * @brief 描画終了
         */
        void EndDraw();

        /**
         * @brief 終了
         */
        void Finalize();

        /**
         * @brief コマンドリストの実行
         */
        void ExecuteCommandList();

        /**
         * @brief GPUの処理待機
         */
        void WaitGPU(int frame);

        /**
         * @brief 現在のフレームのGPU処理待機
         */
        void WaitGPUThisFrame();

        /**
         * @brief 前のフレームのGPU処理待機
         */
        void WaitGPUPrevFrame();

        /**
         * @brief レンダーターゲットの設定
         */
        void SetDefaultRenderTarget() const;

        /**
         * @brief ヒープを設定
         */
        void SetHeaps() const;

        /**
         * @brief デバイスを取得
         */
        ComPtr<ID3D12Device> Device() const;

        /**
         * @brief コマンドリストを取得
         */
        ComPtr<ID3D12GraphicsCommandList> CommandList() const;

        /**
         * @brief コマンドキューを取得
         */
        ComPtr<ID3D12CommandQueue> CommandQueue() const;

        /**
         * @brief ディスクリプタプールを取得
         */
        std::shared_ptr<DescriptorPool> DescriptorPool(PoolType type) const;

        /**
         * @brief 動的定数バッファの取得
         */
        std::shared_ptr<DynamicConstantBuffer> GetConstantBuffer();

        /**
         * @brief リソースデスクを取得
         */
        D3D12_RESOURCE_DESC BackBufferResourceDesc() const;

        /**
         * @brief バックバッファの数を取得
         * @return
        */
        int BackBufferNum() const;

        /**
         * @brief バリアの推移
        */
        void Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const;

        /**
         * @brief 背景色の取得
         * @return 背景色
         */
        static const Color& BackGroundColor();

        /**
         * @brief 背景色の設定
         * @param 背景色
         */
        static void BackGroundColor(const Color& color);

    private:

        /**
         * @brief デバイスの初期化
         */
        bool InitDevice();

        /**
         * @brief コマンドの初期化
         */
        bool InitCommand();

        /**
         * @brief スワップチェーンの作成
         */
        bool CreateSwapChain();

        /**
         * @brief プールの作成
         */
        bool CreateDescriptorPool();

        /**
         * @brief デバッグレイヤーの有効化
         */
        void EnableDebugLayer();
    };
}
