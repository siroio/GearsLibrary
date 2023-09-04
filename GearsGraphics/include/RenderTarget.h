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
         * @brief �X���b�v�`�F�[���p�����_�[�^�[�Q�b�g�̍쐬
         */
        bool Create(uint32_t index, ComPtr<IDXGISwapChain> swapChain);

        /**
         * @brief �����_�[�^�[�Q�b�g�̍쐬
         * @param width ��
         * @param height ����
         * @param renderFormat �J���[�t�H�[�}�b�g
         * @param depthFormat �[�x�o�b�t�@�[�t�H�[�}�b�g
         * @return ���� true
         */
        bool Create(uint32_t width, uint32_t height, DXGI_FORMAT renderFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);

        /**
         * @brief �����_�[�^�[�Q�b�g�̃n���h��
         */
        WeakPtr<Glib::Internal::Graphics::DescriptorHandle> RTVHandle() const;

        /**
         * @brief �[�x�o�b�t�@�̃n���h��
         */
        WeakPtr<Glib::Internal::Graphics::DescriptorHandle> DSVHandle() const;

        /**
         * @brief �����_�[�^�[�Q�b�g
         */
        ComPtr<ID3D12Resource> RenderTargetResource() const;

        /**
         * @brief �[�x�o�b�t�@
         */
        ComPtr<ID3D12Resource> DepthStencilResource() const;

        D3D12_RESOURCE_DESC RTVResourceDesc() const;
        D3D12_RESOURCE_DESC DSVResourceDesc() const;

        /**
         * @brief �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
         */
        DXGI_FORMAT RenderTargetFormat() const;

        /**
         * @brief �[�x�o�b�t�@�̃t�H�[�}�b�g
         */
        DXGI_FORMAT DepthStencilFormat() const;

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
