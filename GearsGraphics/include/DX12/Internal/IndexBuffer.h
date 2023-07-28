#pragma once
#include <d3d12.h>
#include <ComPtr.h>

namespace Glib::Internal::Graphics
{
    class IndexBuffer
    {
    public:
        /**
         * @brief �C���f�b�N�X�o�b�t�@�[�̍쐬
         * @param sizePerIndex �C���f�b�N�X���Ƃ̃T�C�Y
         * @param indexCount �C���f�b�N�X�̃T�C�Y
         * @return ���� true
         */
        bool Create(UINT sizePerIndex, UINT indexCount);

        /**
         * @brief �C���f�b�N�X�o�b�t�@�[���Z�b�g
         * @param rootParameterIndex
         */
        void SetBuffer();

        /**
         * @brief �C���f�b�N�X�o�b�t�@�̍X�V
         * @param size �f�[�^�T�C�Y
         * @param data �f�[�^
         */
        void Update(void* indicies);

    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        D3D12_INDEX_BUFFER_VIEW view_{};
    };
}
