#pragma once

namespace Glib::Internal::Graphics::ShaderCode
{
    /**
     * @brief X方向ブラーシェーダー
     */
    constexpr char XBLUR_VERTEX_SHADER[]
    {
        R"(Texture2D<float4> mainTexture : register(t0);

	    struct VSInput
	    {
		    float4 position : POSITION;
	    };

	    struct VSOutput
	    {
		    float4 position : SV_POSITION;
		    float4 uv[8] : TEXCOORD;
	    };

	    VSOutput VSmain(VSInput input)
	    {
		    VSOutput o;
		    o.position = input.position;
		    o.position.xy = o.position.xy * 2.0f - 1.0f;
		    o.position.y *= -1.0f;

		    float2 texSize;
		    float level;

		    mainTexture.GetDimensions(0, texSize.x, texSize.y, level);
		    float2 tex = input.position.xy;

		    for (int i = 0; i < 8; ++i)
		    {
			    o.uv[i].xy = float2((1.0f + (2.0f * i)) / texSize.x, 0.0f);
			    o.uv[i].zw = o.uv[i].xy * -1.0f;
			    o.uv[i] += float4(tex, tex);
		    }

		    return o;
	    })"
    };

    /**
     * @brief Y方向ブラーシェーダー
     */
    constexpr char YBLUR_VERTEX_SHADER[]
    {
        R"(Texture2D<float4> mainTexture : register(t0);

	    struct VSInput
	    {
		    float4 position : POSITION;
	    };

	    struct VSOutput
	    {
		    float4 position : SV_POSITION;
		    float4 uv[8] : TEXCOORD;
	    };

	    VSOutput VSmain(VSInput input)
	    {
		    VSOutput o;
		    o.position = input.position;
		    o.position.xy = o.position.xy * 2.0f - 1.0f;
		    o.position.y *= -1.0f;

		    float2 texSize;
		    float level;

		    mainTexture.GetDimensions(0, texSize.x, texSize.y, level);
		    float2 tex = input.position.xy;

		    for (int i = 0; i < 8; ++i)
		    {
			    o.uv[i].xy = float2(0.0f, (1.0f + (2.0f * i)) / texSize.y);
			    o.uv[i].zw = o.uv[i].xy * -1.0f;
			    o.uv[i] += float4(tex, tex);
		    }

		    return o;
	    })"
    };

    /**
     * @brief ブラー用ピクセルシェーダー
     */
    constexpr char GAUSSIAN_BLUR_SHADER[]
    {
        R"(cbuffer BlurConstant : register(b0)
	    {
		    float4 weights[2];
	    };

	    Texture2D<float4> mainTexture : register(t0);
	    SamplerState smp : register(s0);

	    struct VSOutput
	    {
		    float4 position : SV_POSITION;
		    float4 uv[8] : TEXCOORD;
	    };

	    typedef VSOutput PSInput;

	    float4 PSmain(PSInput input) : SV_TARGET
	    {
		    float4 color;

		    for (int i = 0; i < 8; ++i)
		    {
			    color += weights[i / 4][i % 4] * mainTexture.Sample(smp, input.uv[i].xy);
			    color += weights[i / 4][i % 4] * mainTexture.Sample(smp, input.uv[i].zw);
		    }

		    return float4(color.xyz, 1.0f);
	    })"
    };
}
