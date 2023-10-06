#pragma once
#include <Internal/Renderer.h>
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/DX12/VertexBuffer.h>
#include <Vector2.h>
#include <Color.h>

namespace Glib::Internal
{
    class CameraBase;
}

namespace Glib
{
    class Transform;
}

namespace Glib
{
    class SpriteRenderer : public Internal::Renderer
    {
    public:
        SpriteRenderer();

        void Start();
        void LateUpdate();
        void Draw(const WeakPtr<Internal::CameraBase>& camera);


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
         * @brief ���E���]���Ă��邩 
         */
        bool FlipX() const;

        /**
         * @brief ���E���]��ݒ�
         */
        void FlipX(bool flip);

        /**
         * @brief �㉺���]���Ă��邩
         */
        bool FlipY() const;

        /**
         * @brief �㉺���]��ݒ�
         */
        void FlipY(bool flip);

        /**
         * @brief �N���b�s���O�ʒu���擾 
         * @brief (�s�N�Z���P��)
         */
        const Vector2& ClippingPosition() const;

        /**
         * @brief �N���b�s���O�ʒu��ݒ�
         * @brief (�s�N�Z���P��)
         */
        void ClippingPosition(const Vector2& position);

        /**
         * @brief �N���b�s���O�T�C�Y���擾
         * @brief (�s�N�Z���P��)
         */
        const Vector2& ClippingSize() const;

        /**
         * @brief �N���b�s���O�T�C�Y��ݒ�
         * @brief (�s�N�Z���P��)
         */
        void ClippingSize(const Vector2& size);

        /**
         * @brief �e�N�X�`��ID���擾
         */
        unsigned int TextureID() const;

        /**
         * @brief �e�N�X�`��ID��ݒ�
         */
        void TextureID(unsigned int id, bool isResetSize = true);

    private:
        bool enabled_{ false };
        Internal::Graphics::ConstantBuffer constantBuffer_{};
        Internal::Graphics::VertexBuffer vertexBuffer_{};
        bool flipX_{ false };
        bool flipY_{ false };
        unsigned int textureID_{ UINT_MAX };
        Vector2 center_{ 0.5f, 0.5f };
        Vector2 textureSize_{ 0.0f, 0.0f };
        Vector2 clippingPosition_{ 0.0f, 0.0f };
        Vector2 clippingSize_{ 0.0f, 0.0f };
        ::Color color_{ Color::White() };
        WeakPtr<Transform> transform_;
    };
}
