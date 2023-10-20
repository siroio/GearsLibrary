#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>

namespace Glib::Internal::Graphics
{
    /**
     * @brief �W���Ŏg�����\�[�X�Ǘ��N���X
     */
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
        bool Initialize();

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
        void SetTexture(unsigned int id, unsigned int rootParameterIndex);

    private:

        /**
         * @brief �e�N�X�`���̍쐬
         */
        bool CreateTexture(unsigned int id, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        /*  ���_�o�b�t�@�쐬  */

        bool CreateCameraVertexBuffer();
        bool CreateImageVertexBuffer();

        /*  �e�N�X�`���쐬  */

        bool CreateWhiteTexture();
        bool CreateMagentaTexture();
        bool CreateNormalMapTexture();

        /*  �p�C�v���C���쐬  */

        bool CreateCameraPipelineState();
        bool CreateSpritePipelineState();
        bool CreateImagePipelineState();
        bool CreateLinePipelineState();
        bool CreateMeshPipelineState();
        bool CreateMeshShadowPipelineState();
        bool CreateSkinnedMeshPipelineState();
        bool CreateSkinnedMeshShadowPipelineState();
    };
}
