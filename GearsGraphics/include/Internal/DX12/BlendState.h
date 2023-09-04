#pragma once

struct D3D12_RENDER_TARGET_BLEND_DESC;

namespace Glib::Internal::Graphics
{
    /**
     * @brief �u�����h�X�e�[�g
     */
    enum class BlendState : unsigned char
    {
        /**
         * @brief ���߂Ȃ�
         */
        Default,

        /**
         * @brief ���u�����h
         */
        Alpha,

        /**
         * @brief ���Z�u�����h
         */
        Add,

        /**
         * @brief ���Z�u�����h
         */
        Substract
    };

    /**
     * @brief D3D12_RENDER_TARGET_BLEND_DESC �쐬�p�N���X
     */
    class BlendDesc
    {
    public:
        static D3D12_RENDER_TARGET_BLEND_DESC Create(BlendState state);
    };
}
