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
         * @brief 中心位置を取得
         */
        const Vector2& Center() const;

        /**
         * @brief 中心位置を設定
         */
        void Center(const Vector2& center);

        /**
         * @brief 色を取得 
         */
        const Color& Color() const;

        /**
         * @brief 色を設定
         */
        void Color(const ::Color& color);

        /**
         * @brief テクスチャIDを取得
         */
        unsigned int TextureID() const;

        /**
         * @brief テクスチャIDを設定
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
