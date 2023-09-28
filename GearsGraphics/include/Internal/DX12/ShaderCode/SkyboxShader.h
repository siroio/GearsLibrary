#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    constexpr char SKYBOX_SHADER[]
    {
        "cbuffer CameraConstant : register(b0)              \
        {                                                   \
            float4x4 View;                                  \
            float4x4 Projetion;                             \
            float4x4 LightVP;                               \
        };                                                  \
                                                            \
        Texture2D<float4> skyboxTex : register(t0);         \
        SamplerState smp : register(s0);                    \
                                                            \
        struct VSInput                                      \
        {                                                   \
            float4 position : POSITION;                     \
            float2 uv : TEXCOORD;                           \
        };                                                  \
                                                            \
        struct VSOutput                                     \
        {                                                   \
            float4 position : SV_POSITION;                  \
            float2 uv : TEXCOORD;                           \
        };                                                  \
                                                            \
        typedef VSOutput PSInput;                           \
                                                            \
        VSOutput VSmain(VSInput input)                      \
        {                                                   \
            VSOutput o;                                     \
            input.position.w = 0.0f;                        \
            float4 viewPosition = mul(View, input.position);\
            viewPosition.w = 1.0f;                          \
            o.position = mul(Projection, viewPosition);     \
            o.uv = input.uv;                                \
            return o;                                       \
        }                                                   \
                                                            \
        float4 PSmain(PSInput input) : SV_TARGET            \
        {                                                   \
            return skyboxTex.Sample(smp, input.uv);         \
        }"
    };
}
