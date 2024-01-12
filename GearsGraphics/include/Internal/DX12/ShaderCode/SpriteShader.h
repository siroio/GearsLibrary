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
            float3 SpritePosition;
            float  SpriteAngle;
            float2 SpriteScale;
            float2 SpriteCenter;
            float2 SpriteSize;
            float2 SpritePadding;
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
            float2 spriteSize = SpriteSize * SpriteScale;
		 	VSOutput o;
		 	o.position = input.position;
		 	o.position.xy *= spriteSize;
		 	o.position.x -= spriteSize.x * SpriteCenter.x;
		 	o.position.y += spriteSize.y * SpriteCenter.y;
		 	float2 rotate;
            float c = cos(SpriteAngle);
            float s = sin(SpriteAngle);
		 	rotate.x = o.position.x * c - o.position.y * s;
		 	rotate.y = o.position.y * c + o.position.x * s;
		 	o.position.xy = rotate;
		 	o.position.xyz += SpritePosition;
		 	o.position = mul(Projection, mul(View, o.position));
		 	o.uv = input.uv;
		 	return o;
        }

        float4 PSmain(PSInput input) : SV_TARGET
        {
            return tex.Sample(smp, input.uv) * SpriteColor;
        })"
    };
}
