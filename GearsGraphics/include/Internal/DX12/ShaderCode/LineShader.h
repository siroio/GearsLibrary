#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    constexpr char LINE_SHADER[]
    {
        "cbuffer CameraConstant : register(b0)                          \
        {                                                               \
            float4x4 View;                                              \
            float4x4 Projection;                                        \
        }                                                               \
                                                                        \
        struct VSInput                                                  \
        {                                                               \
            float4 position : POSITION;                                 \
            float4 color : COLOR;                                       \
        }                                                               \
                                                                        \
        struct VSOutput                                                 \
        {                                                               \
            float4 position : SV_POSITION;                              \
            float color : COLOR                                         \
        }                                                               \
                                                                        \
        typedef VSOutput PSInput;                                       \
                                                                        \
        VSOutput VSmain(VSInput input)                                  \
        {                                                               \
            VSOutput o;                                                 \
            o.position = input.position;                                \
            o.position = mul(Projection, mul(View, output.position));   \
            o.color = input.color;                                      \
            return output;                                              \
        }                                                               \
                                                                        \
        flaot4 PSmain(PSInput input) : SV_TARGET                        \
        {                                                               \
            return input.color;                                         \
        }"
    };
}
