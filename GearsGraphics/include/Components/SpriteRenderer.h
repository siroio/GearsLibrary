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
         * @brief 左右反転しているか
         */
        bool FlipX() const;

        /**
         * @brief 左右反転を設定
         */
        void FlipX(bool flip);

        /**
         * @brief 上下反転しているか
         */
        bool FlipY() const;

        /**
         * @brief 上下反転を設定
         */
        void FlipY(bool flip);

        /**
         * @brief クリッピング位置を取得
         * @brief (ピクセル単位)
         */
        const Vector2& ClippingPosition() const;

        /**
         * @brief クリッピング位置を設定
         * @brief (ピクセル単位)
         */
        void ClippingPosition(const Vector2& position);

        /**
         * @brief クリッピングサイズを取得
         * @brief (ピクセル単位)
         */
        const Vector2& ClippingSize() const;

        /**
         * @brief クリッピングサイズを設定
         * @brief (ピクセル単位)
         */
        void ClippingSize(const Vector2& size);

        /**
         * @brief テクスチャIDを取得
         */
        unsigned int TextureID() const;

        /**
         * @brief テクスチャIDを設定
         */
        void TextureID(unsigned int id, bool isResetSize = true);

    private:
        void OnGUI() override;

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
