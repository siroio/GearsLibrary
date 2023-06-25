#ifndef GEARS_DIRECTX_12_H
#define GEARS_DIRECTX_12_H

#include <Singleton.h>
#include <Internal/ISystem.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>

struct Color;
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct IDXGIFactory6;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct D3D12_RESOURCE_DESC;

namespace Glib::Internal::Graphics
{
    class DirectX12 :
        public SingletonPtr<DirectX12>,
        public Interface::ISystem,
        public Function::InitializeOrderSet<Order::Initialzie::DX12_INIT>,
        public Function::BeginDrawOrderSet<Order::BeginDraw::DX12_BEGINDRAW>
    {
        DirectX12() = default;
        friend WeakPtr<DirectX12> SingletonPtr<DirectX12>::Instance();

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
         * @brief スワップチェーンの作成
         */
        bool CreateSwapChain();

        /**
         * @brief デバッグレイヤーの有効化
         */
        void EnableDebugLayer();
    };
}

#endif // !GEARS_DIRECTX_12_H
