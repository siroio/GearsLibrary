#pragma once

struct D3D12_RENDER_TARGET_BLEND_DESC;

namespace Glib::Internal::Graphics
{
    /**
     * @brief ブレンドステート
     */
    enum class BlendState : unsigned char
    {
        /**
         * @brief 透過なし
         */
        Default,

        /**
         * @brief αブレンド
         */
        Alpha,

        /**
         * @brief 加算ブレンド
         */
        Add,

        /**
         * @brief 減算ブレンド
         */
        Substract
    };

    /**
     * @brief D3D12_RENDER_TARGET_BLEND_DESC 作成用クラス
     */
    class BlendDesc
    {
    public:
        static D3D12_RENDER_TARGET_BLEND_DESC Create(BlendState state);
    };
}
