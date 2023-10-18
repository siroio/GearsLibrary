#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    /**
     * @brief 線描画用シェーダー
     */
    constexpr char LINE_SHADER[]
    {
        R"(cbuffer CameraConstant : register(b0)
        {
            float4x4 View;
            float4x4 Projection;
        };

        struct VSInput
        {
            float4 position : POSITION;
            float4 color : COLOR;
        };

        struct VSOutput
        {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        typedef VSOutput PSInput;

        VSOutput VSmain(VSInput input)                                  
        {
            VSOutput o;
            o.position = input.position;
            o.position = mul(Projection, mul(View, o.position));
            o.color = input.color;
            return o;
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            return input.color;
        })"
    };
}
