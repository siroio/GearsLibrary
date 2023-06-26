#ifndef GEARS_DIRECTX_12_H
#define GEARS_DIRECTX_12_H

#include <Singleton.h>
#include <Internal/ISystem.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
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
         * @brief �R�}���h���X�g�̎��s
        */
        void ExecuteCommandList();

        /**
         * @brief �f�o�C�X���擾
        */
        ComPtr<ID3D12Device> Device() const;

        /**
         * @brief �R�}���h���X�g���擾
        */
        ComPtr<ID3D12GraphicsCommandList> CommandList() const;

        /**
         * @brief �R�}���h�L���[���擾
        */
        ComPtr<ID3D12CommandQueue> CommandQueue() const;

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
         * @brief �R�}���h�̏�����
        */
        bool InitCommand();

        /**
         * @brief �X���b�v�`�F�[���̍쐬
         */
        bool CreateSwapChain();

        /**
         * @brief �o�b�N�o�b�t�@�̍쐬
        */
        bool CreateBackBuffer();

        /**
         * @brief �f�o�b�O���C���[�̗L����
         */
        void EnableDebugLayer();

        /**
         * @brief GPU�̏����ҋ@
        */
        void WaitGPU();
    };
}

#endif // !GEARS_DIRECTX_12_H
