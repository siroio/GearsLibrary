#pragma once
#include <ComPtr.h>

struct D3D12_ROOT_SIGNATURE_DESC;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct ID3D12RootSignature;
struct ID3D12PipelineState;

namespace Glib::Internal::Graphics
{
    /**
     * @brief �O���t�B�b�N�X�p�C�v���C��
     */
    class GraphicsPipeline
    {
    public:

        /**
         * @brief �W���ȃO���t�B�b�N�X�p�C�v���C���̐ݒ���쐬
         */
        static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateDefaultPipelineDesc();

        /**
         * @brief ���[�g�V�O�l�`���̍쐬
         */
        bool CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc);

        /**
         * @brief �p�C�v���C���X�e�[�g�̍쐬
         */
        bool CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);

        /**
         * @brief �O���t�B�b�N�X�p�C�v���C�����Z�b�g
         */
        void SetPipeline();

    private:
        ComPtr<ID3D12RootSignature> rootSignature_;
        ComPtr<ID3D12PipelineState> pipelineState_;
    };

}
