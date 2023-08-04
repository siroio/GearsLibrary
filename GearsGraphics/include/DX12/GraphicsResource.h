#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>

namespace Glib::Graphics
{
    class GraphicsResource :
        public Glib::Internal::Interface::ISystem,
        public SingletonPtr<GraphicsResource>
    {
    private:
        GraphicsResource() = default;
        friend WeakPtr<GraphicsResource> SingletonPtr<GraphicsResource>::Instance();

    public:

        /**
         * @brief ���\�[�X�̏�����
         */
        void Initialize();

        /**
         * @brief ���_�o�b�t�@�̐ݒ�
         */
        void SetVertexBuffer(unsigned int id);

        /**
         * @brief �p�C�v���C���X�e�[�g�̐ݒ�
         */
        void SetPipelineState(unsigned int id);

        /**
         * @brief �e�N�X�`���̐ݒ�
         */
        void SetTexture(unsigned int id);
    private:


    private:
    };
}
