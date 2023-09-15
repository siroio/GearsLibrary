#pragma once

struct Matrix4x4;

namespace Glib
{
    enum class CameraClearFlags;
}

namespace Glib::Internal::Interface
{
    /**
     * @brief カメラインターフェース
     */
    class ICamera
    {
    protected:
        ICamera() = default;

    public:
        /**
         * @brief バックバッファに描画
         */
        virtual void Draw() = 0;

        /**
         * @brief 画面クリアの種類
         */
        virtual CameraClearFlags ClearFlags() = 0;

        /**
         * @brief ビュー行列
         */
        virtual Matrix4x4 ViewMatrix() const = 0;

        /**
         * @brief プロジェクション行列
         */
        virtual Matrix4x4 ProjectionMatrix() const = 0;

        /**
         * @brief レンダーターゲットを設定
         */
        virtual void SetRenderTarget() = 0;

        /**
         * @brief 定数バッファを設定
         * @param rootParamIndex
         */
        virtual void SetConstantBuffer(unsigned int rootParamIndex) = 0;

        /**
         * @brief 深度ステンシルを設定
         */
        virtual void SetDepthStencil() = 0;

        /**
         * @brief シャドウマップを設定
         * @param rootParamIndex
         */
        virtual void SetShadowMap(unsigned int rootParamIndex) = 0;

        /**
         * @brief シャドウマップにブラーをかける
         */
        virtual void ExecuteShadowBulr() = 0;
    };
};
