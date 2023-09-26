#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    /**
     * @brief カメラ用シェーダー
     */
    constexpr char CAMERA_SHADER[]
    {
        "Texture2D<float4> tex : register(t0);              \
		SamplerState smp : register(s0);                    \
                                                            \
		struct VSInput                                      \
		{                                                   \
			float4 position : POSITION;                     \
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
			o.position = input.position;                    \
			o.position.xy = o.position.xy * 2.0f - 1.0f;    \
			o.position.y *= -1;                             \
			o.uv = input.position.xy;                       \
                                                            \
			return o;                                       \
		}                                                   \
                                                            \
		float4 PSmain(PSInput input) : SV_TARGET            \
		{                                                   \
			return tex.Sample(smp, input.uv);               \
		}"
    };
}
