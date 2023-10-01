#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    /**
     * @brief UI画像用シェーダー
     */
    constexpr char IMAGE_SHADER[]
    {
        "cbuffer SpriteConstant : register(b0)                                                                      \
        {                                                                                                           \
            float2 ImagePosition;                                                                                   \
            float2 ImageCenter;                                                                                     \
            float2 ImageScale;                                                                                      \
            float ImageAngle;                                                                                       \
            float Padding;                                                                                          \
            float2 ImageTextureSize;                                                                                \
            float2 WindowSize;                                                                                      \
            float4 ImageColor;                                                                                      \
        };                                                                                                          \
                                                                                                                    \
        Texture2D<float4> tex : register(t0);                                                                       \
        SamplerState smp : register(s0);                                                                            \
                                                                                                                    \
        struct VSInput                                                                                              \
        {                                                                                                           \
            float4 position : POSITION;                                                                             \
        };                                                                                                          \
                                                                                                                    \
        struct VSOutput                                                                                             \
        {                                                                                                           \
            float4 position : SV_POSITION;                                                                          \
            float2 uv : TEXCOORD;                                                                                   \
        };                                                                                                          \
                                                                                                                    \
        typedef VSOutput PSInput;                                                                                   \
                                                                                                                    \
        VSOutput VSmain(VSInput input)                                                                              \
        {                                                                                                           \
            VSOutput o;                                                                                             \
            o.position = input.position;                                                                            \
            o.position.xy *= ImageTextureSize * ImageScale;                                                         \
            o.position.xy -= ImageTextureSize * ImageScale * ImageCenter;                                           \
            float2 rotate;                                                                                          \
            float c = cos(ImageAngle);                                                                              \
            float s = sin(ImageAngle);                                                                              \
            rotate.x = o.position.x * c - o.position.y * s;                                                         \
            rotate.y = o.position.y * c + o.position.x * s;                                                         \
            o.position.xy = rotate + ImagePosition;                                                                 \
            o.position.xy = o.position.xy * float2(2.0f / WindowSize.x, -2.0f / WindowSize.y) + float2(-1.0f, 1.0f);\
            o.uv = input.position;                                                                                  \
                                                                                                                    \
            return o;                                                                                               \
        }                                                                                                           \
                                                                                                                    \
        float4 PSmain(PSInput input) : SV_TARGET                                                                    \
        {                                                                                                           \
            return tex.Sample(smp, input.uv) * ImageColor;                                                          \
        }"
    };
}
