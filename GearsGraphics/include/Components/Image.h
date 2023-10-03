#pragma once
#include <Internal/UIRenderer.h>
#include <Internal/DX12/ConstantBuffer.h>
#include <Vector2.h>
#include <Color.h>

namespace Glib
{
    class Image : public Internal::UIRenderer
    {
    public:
        Image();
        void Start();
        void LateUpdate();

    private:
        virtual void DrawUI() override;

    public:
        /**
         * @brief ���S�ʒu���擾
         */
        const Vector2& Center() const;

        /**
         * @brief ���S�ʒu��ݒ�
         */
        void Center(const Vector2& center);

        /**
         * @brief �F���擾 
         */
        const Color& Color() const;

        /**
         * @brief �F��ݒ�
         */
        void Color(const ::Color& color);

        /**
         * @brief �e�N�X�`��ID���擾
         */
        unsigned int TextureID() const;

        /**
         * @brief �e�N�X�`��ID��ݒ�
         */
        void TextureID(unsigned int id);

    private:
        bool enabled_{ false };
        Internal::Graphics::ConstantBuffer constantBuffer_{};
        unsigned int textureID_{ UINT_MAX };
        Vector2 center_{ 0.5f, 0.5f };
        ::Color color_{ Color::White() };
    };
}
