#pragma once
#include <dxgi1_6.h>
#include <Internal/DX12/DescriptorPool.h>
#include <ComPtr.h>
#include <WeakPtr.h>
#include <Color.h>

namespace Glib::Graphics
{
    class RenderTarget
    {
    public:
        RenderTarget() = default;

        /**
         * @brief レンダーターゲットの作成
         * @param width 幅
         * @param height 高さ
         * @param renderFormat カラーフォーマット
         * @param depthFormat 深度バッファーフォーマット
         * @return 成功 true
         */
        bool Create(uint32_t width, uint32_t height, Color clearColor, DXGI_FORMAT renderFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);

        /**
         * @brief スワップチェーン用レンダーターゲットの作成
         */
        bool Create(uint32_t index, const ComPtr<IDXGISwapChain>& swapChain);

        /**
         * @brief レンダーターゲットのハンドル
         */
        WeakPtr<Glib::Internal::Graphics::DescriptorHandle> RTVHandle() const;

        /**
         * @brief 深度バッファのハンドル
         */
        WeakPtr<Glib::Internal::Graphics::DescriptorHandle> DSVHandle() const;

        /**
         * @brief レンダーターゲット
         */
        ComPtr<ID3D12Resource> RenderTargetResource() const;

        /**
         * @brief 深度バッファ
         */
        ComPtr<ID3D12Resource> DepthStencilResource() const;

        /**
         * @brief レンダーターゲットのリソース設定
        */
        D3D12_RESOURCE_DESC RTVResourceDesc() const;

        /**
         * @brief 深度バッファのリソース設定
        */
        D3D12_RESOURCE_DESC DSVResourceDesc() const;

        /**
         * @brief レンダーターゲットのフォーマット
         */
        DXGI_FORMAT RenderTargetFormat() const;

        /**
         * @brief 深度バッファのフォーマット
         */
        DXGI_FORMAT DepthStencilFormat() const;

        /**
         * @brief テクスチャとして利用
        */
        void AsTexture();

        /**
         * @brief レンダーターゲットとして利用
        */
        void AsRenderTarget();

        /**
         * @brief レンダーターゲットを開放
         */
        void Release();

    private:
        bool CreateRenderTargetBuffer(DXGI_FORMAT format);
        bool CreateDepthStencilBuffer(DXGI_FORMAT format);
        void CreateView(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat);
        RenderTarget(const RenderTarget&) = delete;
        void operator = (const RenderTarget&) = delete;

    private:
        ComPtr<ID3D12Resource> renderTarget_{ nullptr };
        ComPtr<ID3D12Resource> depthStencil_{ nullptr };

        UINT width_{};
        UINT height_{};
        Color clearColor_{ 0.0f, 0.0f, 0.0f, 1.0f };

        std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle> rtvHandle_{ nullptr };
        std::shared_ptr<Glib::Internal::Graphics::DescriptorHandle> dsvHandle_{ nullptr };
    };
}
