#pragma once
#include <ComPtr.h>

struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct D3D12_ROOT_SIGNATURE_DESC;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;

namespace Glib::Internal::Graphics
{
    class GraphicsPipeline
    {
    public:
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
