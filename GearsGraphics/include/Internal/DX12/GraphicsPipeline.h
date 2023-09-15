#pragma once
#include <ComPtr.h>

struct D3D12_ROOT_SIGNATURE_DESC;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct ID3D12RootSignature;
struct ID3D12PipelineState;

namespace Glib::Internal::Graphics
{
    /**
     * @brief グラフィックスパイプライン
     */
    class GraphicsPipeline
    {
    public:

        /**
         * @brief 標準なグラフィックスパイプラインの設定を作成
         */
        static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateDefaultPipelineDesc();

        /**
         * @brief ルートシグネチャの作成
         */
        bool CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc);

        /**
         * @brief パイプラインステートの作成
         */
        bool CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);

        /**
         * @brief グラフィックスパイプラインをセット
         */
        void SetPipeline();

    private:
        ComPtr<ID3D12RootSignature> rootSignature_;
        ComPtr<ID3D12PipelineState> pipelineState_;
    };

}
