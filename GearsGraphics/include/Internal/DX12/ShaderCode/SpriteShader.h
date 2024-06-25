#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    /**
     * @brief スプライト描画用シェーダー
     */
    constexpr char SPRITE_SHADER[]
    {
        R"(cbuffer CameraConstant : register(b0)
        {
            float4x4 View;
            float4x4 Projection;
        };

        cbuffer SpriteConstant : register(b1)
        {
            float4x4 World;
            float2 SpriteCenter;
            float2 SpriteSize;
            float4 SpriteColor;
        };

        Texture2D<float4> tex : register(t0);
        SamplerState smp : register(s0);

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

        VSOutput VSmain(VSInput input)
        {
		 	VSOutput o;
            o.position = input.position;
		 	o.position.xy *= SpriteSize;
            o.position.x -= SpriteCenter.x * SpriteSize.x;
            o.position.y += SpriteCenter.y * SpriteSize.y;
		 	o.position = mul(Projection, mul(View,  mul(World, o.position)));
		 	o.uv = input.uv;
		 	return o;
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            return tex.Sample(smp, input.uv) * SpriteColor;
        })"
    };
}
