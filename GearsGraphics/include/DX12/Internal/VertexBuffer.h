#pragma once
#include <d3d12.h>
#include <ComPtr.h>

namespace Glib::Internal::Graphics
{
    class VertexBuffer
    {
    public:
        /**
         * @brief ���_�o�b�t�@�[�̍쐬
         * @param sizePerIndex ���_���Ƃ̃T�C�Y
         * @param indexCount ���_�̃T�C�Y
         * @return ���� true
         */
        bool Create(UINT sizePerVetex, UINT vertexCount);

        /**
         * @brief ���_�o�b�t�@�[���Z�b�g
         * @param rootParameterIndex
         */
        void SetBuffer();

        /**
         * @brief ���_�o�b�t�@�̍X�V
         * @param size �f�[�^�T�C�Y
         * @param data �f�[�^
         */
        void Update(void* vertices);

    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        D3D12_VERTEX_BUFFER_VIEW view_{};
    };
}
