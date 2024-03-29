﻿#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    /**
* @brief スカイボックス描画用シェーダー
*/
    constexpr char SKYBOX_SHADER[]
    {
        R"(cbuffer CameraConstant : register(b0)
        {
            float4x4 View;
            float4x4 Projection;
            float4x4 LightVP;
        };

        struct VSInput
        {
            float4 position : POSITION;
            float2 uv : TEXCOORD;
        };

        struct VSOutput
        {
            float4 position : SV_POSITION;
            float2 uv : TEXCOORD;
        };

        typedef VSOutput PSInput;

        Texture2D<float4> skyboxTex : register(t0);
        SamplerState smp : register(s0);

        VSOutput VSmain(VSInput input)
        {
            VSOutput o;
            float4 viewPosition = mul(View, float4(input.position.xyz, 0.0f));
            viewPosition.w = 1.0f;
            o.position = mul(Projection, viewPosition);
            o.uv = input.uv;
            return o;
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            return skyboxTex.Sample(smp, input.uv);
        })"
    };
}
