#pragma once

struct Matrix4x4;

namespace Glib
{
    enum class CameraClearFlags;
}

namespace Glib::Internal::Interface
{
    /**
     * @brief �J�����C���^�[�t�F�[�X
     */
    class ICamera
    {
    protected:
        ICamera() = default;

    public:
        /**
         * @brief �o�b�N�o�b�t�@�ɕ`��
         */
        virtual void Draw() = 0;

        /**
         * @brief ��ʃN���A�̎��
         */
        virtual CameraClearFlags ClearFlags() = 0;

        /**
         * @brief �r���[�s��
         */
        virtual Matrix4x4 ViewMatrix() const = 0;

        /**
         * @brief �v���W�F�N�V�����s��
         */
        virtual Matrix4x4 ProjectionMatrix() const = 0;

        /**
         * @brief �����_�[�^�[�Q�b�g��ݒ�
         */
        virtual void SetRenderTarget() = 0;

        /**
         * @brief �萔�o�b�t�@��ݒ�
         * @param rootParamIndex
         */
        virtual void SetConstantBuffer(unsigned int rootParamIndex) = 0;

        /**
         * @brief �[�x�X�e���V����ݒ�
         */
        virtual void SetDepthStencil() = 0;

        /**
         * @brief �V���h�E�}�b�v��ݒ�
         * @param rootParamIndex
         */
        virtual void SetShadowMap(unsigned int rootParamIndex) = 0;

        /**
         * @brief �V���h�E�}�b�v�Ƀu���[��������
         */
        virtual void ExecuteShadowBulr() = 0;
    };
};
