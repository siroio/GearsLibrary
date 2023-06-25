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
         * @brief ������
         */
        bool Initialize();

        /**
         * @brief �`��J�n
         */
        void BeginDraw();

        /**
         * @brief �`��I��
         */
        void EndDraw();

        /**
         * @brief �I��
         */
        void Finalize();

        /**
         * @brief �w�i�F�̎擾
         * @return �w�i�F
         */
        static const Color& BackGroundColor();

        /**
         * @brief �w�i�F�̐ݒ�
         * @param �w�i�F
         */
        static void BackGroundColor(const Color& color);

    private:

        /**
         * @brief �f�o�C�X�̏�����
        */
        bool InitDevice();

        /**
         * @brief �X���b�v�`�F�[���̍쐬
         */
        bool CreateSwapChain();

        /**
         * @brief �f�o�b�O���C���[�̗L����
         */
        void EnableDebugLayer();
    };
}

#endif // !GEARS_DIRECTX_12_H
